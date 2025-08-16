/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:30:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 16:30:00 by hal-moug         ###   ########.fr       */
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
