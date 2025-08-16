/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc7_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 15:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

static int	handle_child_process(int *pipefd, char *delimiter,
			t_shell *shell, int quote)
{
	close(pipefd[0]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	heredoc_child_loop(pipefd[1], delimiter, shell, quote);
	close(pipefd[1]);
	exit(0);
}

static int	handle_heredoc_parent(int *pipefd, pid_t pid, t_shell *shell)
{
	int	status;

	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	ft_restore_main_signals();
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

int	handle_dless(char *delimiter, t_shell *shell, int quote)
{
	int		pipefd[2];
	pid_t	pid;

	if (!validate_delimiter(delimiter) || pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		return (handle_child_process(pipefd, delimiter, shell, quote));
	else
		return (handle_heredoc_parent(pipefd, pid, shell));
}

static int	write_expanded_to_pipe(t_heredoc_data *data, char *expanded_line,
			size_t line_len)
{
	if (data->total_written + line_len + 1 > MAX_HEREDOC_SIZE)
		return (0);
	write(data->pipefd[1], expanded_line, line_len);
	write(data->pipefd[1], "\n", 1);
	data->total_written += line_len + 1;
	return (1);
}

int	process_heredoc_line(char *line, t_heredoc_data *data,
		t_shell *shell)
{
	char	*expanded_line;
	size_t	line_len;

	if (ft_strcmp(line, data->delimiter) == 0)
	{
		free(line);
		return (2);
	}
	expanded_line = expand_variables(line, shell->env, data->quote, &shell->gc);
	if (!expanded_line)
		expanded_line = ft_strdup_gc(&shell->gc, line);
	line_len = ft_strlen(expanded_line);
	if (!write_expanded_to_pipe(data, expanded_line, line_len))
		return (0);
	return (1);
}
