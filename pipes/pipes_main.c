/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 10:43:10 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

static int	wait_for_last_child(t_pipe_data *data, int *final_status)
{
	int	status;

	if (waitpid(data->last_child_pid, &status, WNOHANG) == data->last_child_pid)
	{
		*final_status = status;
		return (1);
	}
	else if (waitpid(data->last_child_pid, &status, 0) == data->last_child_pid)
	{
		*final_status = status;
		return (1);
	}
	return (0);
}

static int	process_waited_children(t_pipe_data *data, int *final_status)
{
	int		status;
	int		flag_quit;
	pid_t	waited_pid;
	int		got_final_status;

	flag_quit = 0;
	got_final_status = 0;
	waited_pid = wait(&status);
	while (waited_pid > 0)
	{
		if (waited_pid == data->last_child_pid)
		{
			*final_status = status;
			got_final_status = 1;
			if (WIFSIGNALED(status))
				handle_signal_output(WTERMSIG(status), &flag_quit);
		}
		waited_pid = wait(&status);
	}
	return (got_final_status);
}

void	wait_for_children(t_pipe_data *data, t_shell *shell)
{
	int	final_status;
	int	got_final_status;

	final_status = 0;
	got_final_status = process_waited_children(data, &final_status);
	if (!got_final_status && data->last_child_pid > 0)
		got_final_status = wait_for_last_child(data, &final_status);
	set_exit_status(shell, final_status, got_final_status);
	cleanup_pipes(data);
}
