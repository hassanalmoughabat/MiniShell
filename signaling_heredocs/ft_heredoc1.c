/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:56:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:11:01 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char *expand_variables(char *line, t_env *env, int quote)
{
    if (quote == 1 || !line || !ft_strchr(line, '$'))
        return ft_strdup(line);
    char *result = malloc(1024);
    if (!result)
        return NULL;
    char *src = line;
    char *dst = result;
    int result_len = 0;
    int buffer_size = 1024;
    while (*src )
    {
        if (*src == '$' && (isalpha(src[1]) || src[1] == '_'))
        {
            src++;
            char var_name[256] = {0};
            int i = 0;
            
            while (*src && (isalnum(*src) || *src == '_') && i < 255)
            {
                var_name[i++] = *src++;
            }
            var_name[i] = '\0';
            char *value = my_getenv(var_name, transform(env));
            if (!value)
                value = "";
            int value_len = ft_strlen(value);          
            if (result_len + value_len + 1 >= buffer_size)
            {
                buffer_size = (result_len + value_len + 1) * 2;
                char *new_result = realloc(result, buffer_size);
                if (!new_result)
                {
                    free(result);
                    return NULL;
                }
                result = new_result;
                dst = result + result_len;
            }
            strcpy(dst, value);
            dst += value_len;
            result_len += value_len;
        }
        else
        {
            if (result_len + 1 >= buffer_size)
            {
                buffer_size *= 2;
                char *new_result = realloc(result, buffer_size);
                if (!new_result)
                {
                    free(result);
                    return NULL;
                }
                result = new_result;
                dst = result + result_len;
            }
            *dst++ = *src++;
            result_len++;
        }
    }
    
    *dst = '\0';
    return (result);
}

int handle_dless(char *delimiter, t_shell *shell, int quote)
{
    int pipefd[2];
    size_t total_written = 0;
    char *line;
    char *expanded_line;
    
    signal(SIGINT, ft_heredoc_sigint_handler);
    if (!validate_delimiter(delimiter) || pipe(pipefd) == -1)
        return (-1);
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            if (line)
                free(line);
            break;
        }
        expanded_line = expand_variables(line, shell->env, quote);
        if (!expanded_line)
            expanded_line = ft_strdup(line);
        size_t line_len = ft_strlen(expanded_line);
        if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
        {
            free(line);
            free(expanded_line);
            close(pipefd[0]);
            close(pipefd[1]);
            return -1;
        }
        write(pipefd[1], expanded_line, line_len);
        write(pipefd[1], "\n", 1);
        total_written += line_len + 1;

        free(line);
        free(expanded_line);
    }

    close(pipefd[1]);
    return pipefd[0];
}

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_check_quotes(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void handle_heredoc(t_shell *shell)
{
    t_token *curr;
    int read_fd;
    pid_t pid;
    int status;
    char *path;
    char *cmd;
    char *delimiter;
    int quote;

    curr = shell->tk;
    while (curr)
    {
        if (curr->cmd && ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            delimiter = get_delimeter(curr);
            if (!delimiter)
                return ;
            quote = has_quotes(delimiter);
            remove_added_quotes(&delimiter);
            cmd = find_command_around_heredoc(shell->tk, delimiter);
            // g_signal.signint_child = true;
            pid = fork();
            if (pid == -1)
            {
                free(delimiter);
                return;
            }
            if (pid == 0)
            {
                signal(SIGINT, ft_sigint_handler);
                signal(SIGQUIT, SIG_IGN);
                if (cmd)
                {
                    char *args[] = {cmd, NULL};
                    path = get_path(cmd, shell->ft_env);
                    read_fd = handle_dless(delimiter, shell, quote);
                    if (read_fd < 0)
                    {
                        free(delimiter);
                        exit(1);
                    }
                    dup2(read_fd, STDIN_FILENO);
                    close(read_fd);
                    execve(path, args, shell->ft_env);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    read_fd = handle_dless(delimiter, shell, quote);
                    if (read_fd < 0)
                    {
                        free(delimiter);
                        exit(1);
                    }
                    close(read_fd);
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
                free(delimiter);
                waitpid(pid, &status, 0);
                g_signal.signint_child = false;
                if (WIFSIGNALED(status))
                {
                    shell->env->exit_status = 128 + WTERMSIG(status);
                    if (WTERMSIG(status) == SIGINT)
                    {
                        g_signal.heredoc_sigint = true;
                        return; 
                    }
                }
                else
                    shell->env->exit_status = WEXITSTATUS(status);
                curr = curr->next->next;
                continue;
            }
        }
        curr = curr->next;
    }
}
