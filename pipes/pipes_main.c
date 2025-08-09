/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/08/09 15:22:08 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	set_exit_status(t_shell *shell, int final_status, int got_final_status)
{
	if (got_final_status)
	{
		if (WIFEXITED(final_status))
			shell->env->exit_status = WEXITSTATUS(final_status);
		else if (WIFSIGNALED(final_status))
			shell->env->exit_status = 128 + WTERMSIG(final_status);
	}
	else
		shell->env->exit_status = 1;
	g_signal.sig_status = shell->env->exit_status;
}

void	cleanup_pipes(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (i < data->pipe_count)
	{
		free(data->pipes[i]);
		i++;
	}
	free(data->pipes);
}

void	handle_signal_output(int sig, int *flag_quit)
{
	if (sig == SIGINT && !(*flag_quit))
	{
		write(STDERR_FILENO, "\n", 1);
		*flag_quit = 1;
	}
	else if (sig == SIGQUIT && !(*flag_quit))
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		*flag_quit = 1;
	}
}

// //////////////////////////////////// NEWWWWWWWW///////////////////
void	wait_for_children(t_pipe_data *data, t_shell *shell)
{
	int		status;
	int		final_status;
	int		flag_quit;
	pid_t	waited_pid;
	int		got_final_status;

	final_status = 0;
	flag_quit = 0;
	got_final_status = 0;
	waited_pid = wait(&status);
	while (waited_pid > 0)
	{
		if (waited_pid == data->last_child_pid)
		{
			final_status = status;
			got_final_status = 1;
			if (WIFSIGNALED(status))
				handle_signal_output(WTERMSIG(status), &flag_quit);
		}
		waited_pid = wait(&status);
	}
	set_exit_status(shell, final_status, got_final_status);
	cleanup_pipes(data);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/07/23 11:52:23 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	wait_for_children(t_pipe_data *data)
{
	int	status;
	int	i;

	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		data->env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->env->exit_status = 128 + WTERMSIG(status);
	i = 0;
	while (i < data->pipe_count)
	{
		free(data->pipes[i]);
		i++;
	}
	free(data->pipes);
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

void	close_parent_heredoc(t_pipe_data *data, int heredoc_fd)
{
	int	j;

	if (heredoc_fd < 0)
		return ;
	j = 0;
	while (j < data->hd_count)
	{
		if (data->heredocs[j].fd == heredoc_fd)
		{
			close(data->heredocs[j].fd);
			data->heredocs[j].fd = -1;
			break ;
		}
		j++;
	}
}
