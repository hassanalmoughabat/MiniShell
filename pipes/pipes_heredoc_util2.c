/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc_util2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 02:43:10 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/16 10:16:27 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	handle_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->env->exit_status = 128 + WTERMSIG(status);
	if (WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

int	handle_heredoc_pipe_redirect_part2(t_heredoc_pipe_params *params,
			t_shell *shell)
{
	pid_t	pid2;
	int		status1;
	int		status2;

	pid2 = create_output_child(params->pipefd, params->redirect_token,
			params->ft_env);
	if (pid2 == -1)
	{
		close(params->pipefd[0]);
		close(params->pipefd[1]);
		free(params->delimiter);
		kill(params->pid1, SIGTERM);
		waitpid(params->pid1, NULL, 0);
		return (0);
	}
	close(params->pipefd[0]);
	close(params->pipefd[1]);
	free(params->delimiter);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(params->pid1, &status1, 0);
	if (WIFSIGNALED(status1) && WTERMSIG(status1) == SIGINT)
	{
		kill(pid2, SIGKILL);
		waitpid(pid2, &status2, 0);
		shell->env->exit_status = 130;
		ft_restore_main_signals();
		return (1);
	}
	if (WIFEXITED(status1) && WEXITSTATUS(status1) == 130)
	{
		kill(pid2, SIGKILL);  // Kill output child immediately  
		waitpid(pid2, &status2, 0);
		shell->env->exit_status = 130;
		ft_restore_main_signals();
		return (1);
	}
	waitpid(pid2, &status2, 0);
	handle_exit_status(shell, status2);
	ft_restore_main_signals();
	return (1);
}

int	setup_heredoc_for_command(t_pipe_data *data, t_token *start,
			t_token *curr)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	if (data->heredocs && segment_has_heredoc(start, curr))
	{
		heredoc_fd = get_heredoc_fd_for_segment(start, curr,
				data->heredocs, data->hd_count);
	}
	return (heredoc_fd);
}

void	close_unused_heredocs(t_pipe_data *data, int heredoc_fd)
{
	int	j;

	if (!data->heredocs)
		return ;
	j = 0;
	while (j < data->hd_count)
	{
		if (data->heredocs[j].fd != heredoc_fd
			&& data->heredocs[j].fd >= 0)
			close(data->heredocs[j].fd);
		j++;
	}
}

void	setup_child_data(t_pipe_child_data *child_data, t_pipe_data *data,
			t_child_setup_params *params)
{
	child_data->cmd_segment = params->cmd_segment;
	child_data->ft_env = data->ft_env;
	child_data->env = data->env;
	child_data->i = params->i;
	child_data->pipes = data->pipes;
	child_data->pipe_count = data->pipe_count;
	child_data->heredoc_fd = params->heredoc_fd;
	child_data->is_first_with_heredoc = (params->i == 0
			&& params->heredoc_fd >= 0);
}
