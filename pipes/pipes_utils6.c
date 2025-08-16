/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:00:05 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/16 10:32:53 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	execute_all_commands(t_pipe_data *data, t_shell *shell)
{
	t_single_cmd_params	params;
	t_token				*curr;
	t_token				*start;
	int					i;

	start = data->lst;
	curr = next_pipe(start);
	i = 0;
	params.data = data;
	params.shell = shell;
	while (1)
	{
		params.start = start;
		params.curr = curr;
		params.i = i;
		execute_single_command(&params);
		if (!curr)
			break ;
		start = curr->next;
		curr = next_pipe(start);
		i++;
	}
	close_all_pipes(data->pipes, data->pipe_count);
}

void	execute_pipe_commands(t_pipe_data *data, t_shell *shell)
{
	setup_pipe_signals();
	execute_all_commands(data, shell);
	cleanup_heredocs_after_exec(data);
	wait_for_children(data, shell);
	restore_pipe_signals();
	ft_restore_parent_signals();
}
