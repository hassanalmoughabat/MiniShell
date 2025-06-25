/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:56:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/25 22:45:58 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	get_exit_code(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

// static void setup_exec_signals(void)
// {
//     signal(SIGINT, SIG_DFL);
//     signal(SIGQUIT, SIG_DFL);
// }

static void	ft_heredoc_sigint_handler(int signum)
{
	 (void)signum;
    exit(128 + SIGINT);
}

char *extract_content_without_quotes(char *str, char quote_type)
{
    int     i;
    int     j;
    int     len;
    char    *result;

    if (!str)
        return (NULL);
    len = ft_strlen(str);
    if (len < 2)
        return (ft_strdup(str));
    if (str[0] == quote_type && str[len - 1] == quote_type)
    {
        result = (char *)malloc(sizeof(char) * (len - 1));
        if (!result)
            return (NULL);
        
        i = 1;
        j = 0;
        while (i < len - 1)
            result[j++] = str[i++];
        result[j] = '\0';
        return (result);
    }
    return (ft_strdup(str));
}

char *ft_strsub(char const *s, unsigned int start, size_t len)
{
    char    *str;
    size_t  i;

    if (!s)
        return (NULL);
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        str[i] = s[start + i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

char *get_delimeter(t_token *tk)
{
    t_token *curr;
    char    *delimeter;
    int     len;

    curr = tk;
    while (curr)
    {
        if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            delimeter = curr->next->cmd;
            len = ft_strlen(delimeter);
            if (len >= 2 && delimeter[0] == '\'' && delimeter[len - 1] == '\'')
                return (ft_strsub(delimeter, 1, len - 2));
            else if (len >= 2 && delimeter[0] == '\"' && delimeter[len - 1] == '\"')
                return (ft_strsub(delimeter, 1, len - 2));
            else
                return (ft_strdup(delimeter));

        }
        curr = curr->next;
    }
    return (NULL);
}
char *find_command_around_heredoc(t_token *tk, char *delimiter)
{
    t_token *curr = tk;
    t_token *command_before = NULL;
    
    while (curr)
    {
        if (curr->next && ft_strcmp(curr->next->cmd, "<<") == 0)
            command_before = curr;

        if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            curr = curr->next;
            while (curr)
            {
                if (ft_strcmp(curr->cmd, delimiter) == 0 && curr->next)
                    return curr->next->cmd;
                curr = curr->next;
            }
            if (command_before)
                return command_before->cmd;
                
            return NULL;
        }
        curr = curr->next;
    }
    return NULL;  
}

int is_delimeter_quoted(t_token *tk)
{
    t_token *curr;
    char    *delimeter;
    int     i;

    curr = tk;
    while (curr)
    {
        if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            delimeter = curr->next->cmd;
            i = 0;
            while (delimeter[i])
            {
                if (delimeter[i] == '\'' || delimeter[i] == '\"')
                    return (1);
                i++;
            }
        }
        curr = curr->next;
    }
    return (0);
}


char *process_all_variables(char *line, t_env *env)
{
    char *result = ft_strdup(line);
    char *temp_result;
    char *var_start;
    char *var_end;
    char *var_name;
    char *var_value;
    size_t var_len;
    
    if (!result)
        return NULL;
    while ((var_start = strchr(result, '$')) != NULL)
    {
        var_end = var_start + 1;
        while (*var_end && (isalnum(*var_end) || *var_end == '_'))
            var_end++;
        var_len = var_end - var_start;
        var_name = malloc(var_len + 1);
        if (!var_name)
        {
            free(result);
            return NULL;
        }
        strncpy(var_name, var_start, var_len);
        var_name[var_len] = '\0';
        var_value = cut_from_op('$', var_start, env);
        if (!var_value)
            var_value = ft_strdup(""); 
        temp_result = replace_variable(result, var_name, var_value);
        free(result);
        free(var_name);
        free(var_value);
        if (!temp_result)
            return NULL;
        result = temp_result;
    }
    
    return result;
}

char *extract_variable_quote(char *line)
{
    if (!line)
        return NULL;
    size_t start = 0;
    size_t end = strlen(line);
    while (line[start] == '\'' && start < end)
        start++;
    while (end > start && line[end - 1] == '\'')
        end--;
    size_t len = end - start;
    if (len == 0)
        return NULL;
    char *result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    strncpy(result, line + start, len);
    result[len] = '\0';

    return result;
}

// static int is_quoted_format(const char *line)
// {
//     if (!line)
//         return 0;
//     size_t len = strlen(line);
//     if (len >= 4 && line[0] == '\'' && line[1] == '\'' && line[len - 2] == '\'' && line[len - 1] == '\'')
//         return 1;
//     return 0;
// }

char *expand_variables(char *line, t_env *env, int quote)
{
    if (!line || !strchr(line, '$'))
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
        if (quote == 0 && *src == '$' && (isalpha(src[1]) || src[1] == '_'))
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
            
            int value_len = strlen(value);          
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
    return result;
}

int handle_dless(char *delimiter, t_env *env, int flag, int quote)
{
    int pipefd[2];
    size_t total_written = 0;
    char *line;
    char *expanded_line;
    (void)flag;

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
        expanded_line = expand_variables(line, env, quote);
        if (!expanded_line)
            expanded_line = ft_strdup(line);

        size_t line_len = strlen(expanded_line);

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
void handle_heredoc(char **ft_env, t_env *env, t_token *tk)
{
    t_token *curr;
    int read_fd;
    pid_t pid;
    int status;
    char *path;
    char *cmd;
    char *delimiter;
    int quote;

    curr = tk;
    while (curr)
    {
        if (curr->cmd && ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            delimiter = get_delimeter(curr);
            if (!delimiter)
                return;
                
            quote = is_delimeter_quoted(curr);
            cmd = find_command_around_heredoc(tk, delimiter);

            g_minishell.signint_child = true;
            pid = fork();
            
            if (pid == -1)
            {
                free(delimiter);
                return;
            }
            else if (pid == 0)
            {
                signal(SIGINT, ft_sigint_handler);
                signal(SIGQUIT, SIG_IGN);
                
                if (cmd)
                {
                    char *args[] = {cmd, NULL};
                    path = get_path(cmd, ft_env);
                    read_fd = handle_dless(delimiter, env, 1, quote);
                    if (read_fd < 0)
                    {
                        free(delimiter);
                        exit(1);
                    }
                    
                    dup2(read_fd, STDIN_FILENO);
                    close(read_fd);
                    execve(path, args, ft_env);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    read_fd = handle_dless(delimiter, env, 0, quote);
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
                g_minishell.signint_child = false;
                
                if (WIFSIGNALED(status))
                {
                    env->exit_status = 128 + WTERMSIG(status);
                    if (WTERMSIG(status) == SIGINT)
                    {
                        g_minishell.heredoc_sigint = true;
                        return; 
                    }
                }
                else
                    env->exit_status = WEXITSTATUS(status);
                curr = curr->next->next;
                continue;
            }
        }
        curr = curr->next;
    }
}

