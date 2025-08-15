/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:25:34 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/15 01:08:53 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include <bits/types/sigset_t.h>
#include <asm-generic/signal-defs.h>

void	print_eof_warning(char *delimiter)
{
	ft_putstr_fd("bash: warning: here-document delimited by ", 2);
	ft_putstr_fd("end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
}

// int	write_line_to_pipe(t_heredoc_data *data, char *expanded_line)
// {
// 	size_t	line_len;

// 	line_len = ft_strlen(expanded_line);
// 	if (data->total_written + line_len + 1 > MAX_HEREDOC_SIZE)
// 	{
// 		ft_putendl_fd("minishell: heredoc: maximum size exceeded", 2);
// 		close(data->pipefd[0]);
// 		close(data->pipefd[1]);
// 		ft_init_signals();
// 		return (0);
// 	}
// 	write(data->pipefd[1], expanded_line, line_len);
// 	write(data->pipefd[1], "\n", 1);
// 	data->total_written += line_len + 1;
// 	return (1);
// }

// int	process_heredoc_line(char *line, t_heredoc_data *data,
// 		t_shell *shell)
// {
// 	char	*expanded_line;
// 	int		result;

// 	if (ft_strcmp(line, data->delimiter) == 0)
// 	{
// 		free(line);
// 		return (2);
// 	}
// 	expanded_line = expand_variables(line, shell->env, data->quote);
// 	if (!expanded_line)
// 		expanded_line = ft_strdup(line);
// 	result = write_line_to_pipe(data, expanded_line);
// 	free(line);
// 	free(expanded_line);
// 	if (!result)
// 		return (0);
// 	return (1);
// }

void	heredoc_child_loop(int write_fd, char *delimiter, t_shell *shell, int quote)
{
	char	*line;
	char	*expanded_line;
	size_t	total_written = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_eof_warning(delimiter);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		expanded_line = expand_variables(line, shell->env, quote);
		if (!expanded_line)
			expanded_line = ft_strdup(line);
		size_t line_len = ft_strlen(expanded_line);
		if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
		{
			ft_putendl_fd("minishell: heredoc: maximum size exceeded", 2);
			free(line);
			free(expanded_line);
			exit(1);
		}
		write(write_fd, expanded_line, line_len);
		write(write_fd, "\n", 1);
		total_written += line_len + 1;
		free(line);
		free(expanded_line);
	}
}

// int	heredoc_loop(t_heredoc_data *data, t_shell *shell, int is_child_process)
// {
// 	char	*line;
// 	int		process_result;

// 	while (1)
// 	{
// 		if (!is_child_process && g_signal.heredoc_sigint)
// 		{
// 			close(data->pipefd[0]);
// 			close(data->pipefd[1]);
// 			return (shell->env->exit_status = 130, -1);
// 		}
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			if (is_child_process && g_signal.heredoc_sigint)
// 			{
// 				close(data->pipefd[0]);
// 				close(data->pipefd[1]);
// 				exit(130);
// 			}
// 			if (!is_child_process && g_signal.heredoc_sigint)
// 			{
// 				close(data->pipefd[0]);
// 				close(data->pipefd[1]);
// 				return (shell->env->exit_status = 130, -1);
// 			}
// 			print_eof_warning(data->delimiter);
// 			break;
// 		}
// 		if (!is_child_process && g_signal.heredoc_sigint)
// 		{
// 			free(line);
// 			close(data->pipefd[0]);
// 			close(data->pipefd[1]);
// 			return (shell->env->exit_status = 130, -1);
// 		}
// 		process_result = process_heredoc_line(line, data, shell);
// 		if (process_result == 2)
// 			break;
// 		if (process_result == 0)
// 			return (0);
// 	}
// 	return (1);
// }


int	handle_dless(char *delimiter, t_shell *shell, int quote, int setup_signals)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	
	if (!validate_delimiter(delimiter) || pipe(pipefd) == -1)
		return (-1);
	
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	
	if (pid == 0) // Child process
	{
		close(pipefd[0]);
		
		// Child uses DEFAULT signal handling - will be killed by Ctrl+C
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		
		// Simple heredoc loop in child
		heredoc_child_loop(pipefd[1], delimiter, shell, quote);
		
		close(pipefd[1]);
		exit(0);
	}
	else // Parent process
	{
		close(pipefd[1]);
		
		// Parent ignores signals while waiting
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		
		waitpid(pid, &status, 0);
		
		// Restore original signal handlers
		if (setup_signals)
			ft_restore_main_signals();
		
		// Check how child exited
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipefd[0]);
			shell->env->exit_status = 130;
			write(STDOUT_FILENO, "\n", 1);
			return (-1);
		}
		
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			close(pipefd[0]);
			shell->env->exit_status = WEXITSTATUS(status);
			return (-1);
		}
		
		return (pipefd[0]);
	}
}


// int	handle_dless(char *delimiter, t_shell *shell, int quote, int setup_signals)
// {
// 	t_heredoc_data	data;
// 	int				result;

// 	if (!validate_delimiter(delimiter) || pipe(data.pipefd) == -1)
// 		return (-1);
// 	data.total_written = 0;
// 	data.delimiter = delimiter;
// 	data.quote = quote;
// 	if (setup_signals)
// 	{
// 		ft_set_heredoc_signals();
// 		g_signal.heredoc_sigint = false;
// 	}
// 	result = heredoc_loop(&data, shell, (setup_signals == 0));
// 	if (result == -1)
// 		return (-1);
// 	if (!result)
// 	{
// 		close(data.pipefd[0]);
// 		close(data.pipefd[1]);
// 		if (setup_signals)
// 			ft_init_signals();
// 		return (-1);
// 	}
// 	close(data.pipefd[1]);
// 	if (setup_signals)
// 		ft_init_signals();
// 	return (data.pipefd[0]);
// }
