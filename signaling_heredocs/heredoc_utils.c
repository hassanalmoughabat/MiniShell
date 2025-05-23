// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   heredoc_utils.c                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/02 17:33:08 by njoudieh42        #+#    #+#             */
// /*   Updated: 2025/05/18 16:55:00 by njoudieh42       ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../includes/minihell.h"

// static void	ft_heredoc_sigint_handler(int signum)
// {
// 	(void)signum;
// 	// ft_clean_ms();
// 	exit(SIGINT);
// }

// static int	is_delimeter_quoted(t_token *tk)
// {
// 	t_token	*curr;
// 	char	*delimeter;
// 	int		i;

// 	curr = tk;
// 	while (curr)
// 	{
// 		if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
// 		{
// 			delimeter = curr->next->cmd;
// 			i = 0;
// 			while (delimeter[i])
// 			{
// 				if (delimeter[i] == '\'' || delimeter[i] == '\"')
// 					return (1);
// 				i++;
// 			}
// 		}
// 		curr = curr->next;
// 	}
// 	return (0);
// }

// char	*extract_variable_quote(char *line)
// {
// 	size_t	start;
// 	size_t	len;
// 	size_t	end;
// 	char	*result;

// 	start = 0;
// 	end = ft_strlen(line);
// 	if (!line)
// 		return (NULL);
// 	while (line[start] == '\'' && start < end)
// 		start++;
// 	while (end > start && line[end - 1] == '\'')
// 		end--;
// 	len = end - start;
// 	if (len == 0)
// 		return (NULL);
// 	result = (char *)malloc(len + 1);
// 	if (!result)
// 		return (NULL);
// 	strncpy(result, line + start, len);
// 	result[len] = '\0';
// 	return (result);
// }

// int	handle_dless(char *delimiter, t_env *env, int flag, int quote)
// {
// 	int		pipefd[2];
// 	size_t	line_len;
// 	size_t	total_written = 0;
// 	char	*line;
// 	char	*inner_content;
// 	char	*val;
// 	char	*oldval;
// 	char	*processed_line;

// 	processed_line = NULL;
// 	if (!validate_delimiter(delimiter))
// 		return (-1);
// 	if (pipe(pipefd) == -1)
// 		return (-1);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (quote == 0 && contain_char(line, '$')
// 			&& flag == 1 && !contain_char(line, '\"'))
// 		{
// 			if (is_quoted_format(line))
// 			{
// 				inner_content = ft_strsub(line, 2, strlen(line) - 4);
// 				oldval = extract_variable(inner_content);
// 				if (oldval)
// 				{
// 					val = cut_from_op('$', inner_content, env);
// 					if (val)
// 					{
// 						processed_line = malloc(strlen(val) + 3);
// 						if (processed_line)
// 						{
// 							processed_line[0] = '\'';
// 							strcpy(processed_line + 1, val);
// 							processed_line[strlen(val) + 1] = '\'';
// 							processed_line[strlen(val) + 2] = '\0';
// 						}
// 						free(inner_content);
// 					}
// 					free(oldval);
// 				}
// 				if (!processed_line)
// 					processed_line = ft_strdup(line);
// 			}
// 			else if (line[0] == '\'' && line[strlen(line) - 1] == '\'')
//             {
//                 inner_content = ft_strsub(line, 1, strlen(line) - 2);
//                 oldval = extract_variable(inner_content);
//                 if (oldval)
//                 {
//                     val = cut_from_op('$', inner_content, env);
//                     processed_line = ft_strdup(val ? val : "");
//                     free(oldval);
//                 }
//                 free(inner_content);
//                 if (!processed_line)
//                     processed_line = ft_strdup(line);
//             }
//             else
//             {
//                 oldval = extract_variable(line);
//                 if (oldval)
//                 {
//                     val = cut_from_op('$', line, env);
//                     if (val)
//                     {
//                         processed_line = replace_variable(line, oldval, val);
//                         free(oldval);
//                     }
//                 }
//             }
//         }
//         char *final_line;
//         if (processed_line)
//             final_line = processed_line;
//         else
//             final_line = line;
// 		line_len = ft_strlen(line);
// 		if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
// 		{
// 			free(line);
// 			close(pipefd[0]);
// 			close(pipefd[1]);
// 			return (-1);
// 		}
// 		write(pipefd[1], line, line_len);
// 		write(pipefd[1], "\n", 1);
// 		total_written += line_len + 1;
// 		free (line);
// 	}
// 	close(pipefd[1]);
// 	return (pipefd[0]);
// }

// void	handle_heredoc(char **ft_env, t_env *env, t_token *tk)
// {
// 	int		read_fd;
// 	pid_t	pid;
// 	int		status;
// 	char	*path;
// 	char	*cmd;
// 	char	*delimeter;
// 	int		quote;

// 	quote = is_delimeter_quoted(tk);
// 	delimeter = get_delimeter(tk);
// 	cmd = find_command_around_heredoc(tk, delimeter);
// 	if (cmd)
// 	{
// 		char *args[] = {cmd, NULL};
// 		path = get_path(cmd, ft_env);
// 		read_fd = handle_dless(delimeter, env, 1, quote);
// 		if (!read_fd)
// 			return ;
// 		pid = fork();
// 		signal(SIGQUIT, SIG_IGN);
// 		if (pid == -1)
// 			return ;
// 		else if (pid == 0)
// 		{
// 			signal(SIGINT, ft_heredoc_sigint_handler);
// 			dup2(read_fd, STDIN_FILENO);
// 			close(read_fd);
// 			execve(path, args, ft_env);
// 			// ft_clean_all();
// 			exit (0);
// 		}
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			signal(SIGQUIT, ft_sigquit_handler);
// 			g_minishell.signint_child = false;
// 			close(read_fd);
// 			if (WIFEXITED(pid) && WEXITSTATUS(pid) == SIGINT)
// 				return ;
// 		}
// 	}
// 	else
// 		handle_dless(delimeter, env, 0, quote);
// }
