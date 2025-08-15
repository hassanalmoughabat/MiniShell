/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/13 00:06:41 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	setup_pipe_signals(void)
{
	g_signal.signint_child = true;
}

void	restore_pipe_signals(void)
{
	g_signal.signint_child = false;
}

static void	close_parent_pipes(t_pipe_data *data, int current_cmd)
{
	if (current_cmd > 0)
		close(data->pipes[current_cmd - 1][0]);
	if (current_cmd < data->pipe_count)
		close(data->pipes[current_cmd][1]);
}

void	setup_child_process(t_single_cmd_params *params, t_token *cmd_segment,
				int heredoc_fd)
{
	t_pipe_child_data		child_data;
	t_child_setup_params	setup_params;

	ft_set_child_signals();
	close_unused_heredocs(params->data, heredoc_fd);
	setup_params.cmd_segment = cmd_segment;
	setup_params.i = params->i;
	setup_params.heredoc_fd = heredoc_fd;
	setup_child_data(&child_data, params->data, &setup_params);
	handle_pipe_child(&child_data, params->shell);
	free_token_list(cmd_segment);
	exit(EXIT_FAILURE);
}

void	execute_single_command(t_single_cmd_params *params)
{
	t_token	*cmd_segment;
	pid_t	pid;
	int		heredoc_fd;

	heredoc_fd = setup_heredoc_for_command(params->data, params->start,
			params->curr);
	cmd_segment = extract_command_segment(params->start, params->curr, &params->shell->gc);
	if (!cmd_segment)
		return ;
	pid = fork();
	if (pid == 0)
		setup_child_process(params, cmd_segment, heredoc_fd);
	params->data->last_child_pid = pid;
	close_parent_pipes(params->data, params->i);
	free_token_list(cmd_segment);
}
