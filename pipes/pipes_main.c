/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/08/09 02:56:55 by njoudieh42       ###   ########.fr       */
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
