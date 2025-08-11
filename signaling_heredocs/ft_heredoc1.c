/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:56:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/09 16:04:08 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*expand_variables(char *line, t_env *env, int quote)
{
	char	*result;
	char	*src;
	char	*dst;
	int		result_len;
	int		buffer_size;
	char	var_name[256] = {0};
	int		i;
	char	*value;
	int		value_len;
	char	*new_result;

	if (quote == 1 || !line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	result = malloc(1024);
	if (!result)
		return (NULL);
	src = line;
	dst = result;
	result_len = 0;
	buffer_size = 1024;
	while (*src)
	{
		if (*src == '$' && (isalpha(src[1]) || src[1] == '_'))
		{
			src++;
			i = 0;
			while (*src && (isalnum(*src) || *src == '_') && i < 255)
			{
				var_name[i++] = *src++;
			}
			var_name[i] = '\0';
			value = my_getenv(var_name, transform(env));
			if (!value)
				value = "";
			value_len = ft_strlen(value);
			if (result_len + value_len + 1 >= buffer_size)
			{
				buffer_size = (result_len + value_len + 1) * 2;
				new_result = realloc(result, buffer_size);
				if (!new_result)
				{
					free(result);
					return (NULL);
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
				new_result = realloc(result, buffer_size);
				if (!new_result)
				{
					free(result);
					return (NULL);
				}
				result = new_result;
				dst = result + result_len;
			}
			*dst++ = *src++;
			result_len++;
		}
	}
	*dst = '\0';
	free(line);
	return (result);
}

int	handle_dless(char *delimiter, t_shell *shell, int quote)
{
	int		pipefd[2];
	size_t	total_written;
	char	*line;
	char	*expanded_line;
	size_t	line_len;

	total_written = 0;
	if (!validate_delimiter(delimiter) || pipe(pipefd) == -1)
		return (-1);
	ft_set_heredoc_signals();
	g_signal.heredoc_sigint = false;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("bash:heredoc delimited by EOF (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putendl_fd("')", 2);
			shell->env->exit_status = 0;
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = expand_variables(line, shell->env, quote);
		if (!expanded_line)
			expanded_line = ft_strdup(line);
		line_len = ft_strlen(expanded_line);
		if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
		{
			ft_putendl_fd("minishell: heredoc: maximum size exceeded", 2);
			free(line);
			free(expanded_line);
			close(pipefd[0]);
			close(pipefd[1]);
			ft_init_signals();
			return (-1);
		}
		write(pipefd[1], expanded_line, line_len);
		write(pipefd[1], "\n", 1);
		total_written += line_len + 1;
		free(line);
		free(expanded_line);
	}
	close(pipefd[1]);
	ft_init_signals();
	return (pipefd[0]);
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

void	handle_heredoc(t_shell *shell)
{
	t_token	*curr;
	int		read_fd;
	pid_t	pid;
	int		status;
	char	*path;
	char	*cmd;
	char	*delimiter;
	int		quote;
	char	*args[] = {cmd, NULL};

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
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				free(delimiter);
				shell->env->exit_status = 1;
				return ;
			}
			if (pid == 0)
			{
				read_fd = handle_dless(delimiter, shell, quote);
				if (read_fd < 0)
				{
					free(delimiter);
					exit(1);
				}
				if (cmd)
				{
					path = get_path(cmd, shell->ft_env);
					if (dup2(read_fd, STDIN_FILENO) == -1)
					{
						perror("dup2");
						close(read_fd);
						free(delimiter);
						exit(1);
					}
					close(read_fd);
					if (path)
					{
						execve(path, args, shell->ft_env);
						free(path);
					}
					free(delimiter);
					exit(127);
				}
				else
				{
					close(read_fd);
					free(delimiter);
					exit(0);
				}
			}
			else
			{
				free(delimiter);
				waitpid(pid, &status, 0);
				if (WIFSIGNALED(status))
					shell->env->exit_status = 128 + WTERMSIG(status);
				else
					shell->env->exit_status = WEXITSTATUS(status);
				if (shell->env->exit_status == 130)
					return ;
				curr = curr->next->next;
				continue ;
			}
		}
		curr = curr->next;
	}
}
