/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 10:26:47 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	setup_pipe_signals(void)
{
	g_signal.signint_child = true;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_pipe_signals(void)
{
	g_signal.signint_child = false;
	ft_restore_main_signals();
}

void	close_parent_pipes(t_pipe_data *data, int current_cmd)
{
	if (current_cmd > 0 && data->pipes[current_cmd - 1][0] != -1)
	{
		close(data->pipes[current_cmd - 1][0]);
		data->pipes[current_cmd - 1][0] = -1;
	}
	if (current_cmd < data->pipe_count && data->pipes[current_cmd][1] != -1)
	{
		close(data->pipes[current_cmd][1]);
		data->pipes[current_cmd][1] = -1;
	}
}


void	setup_child_process(t_single_cmd_params *params, t_token *cmd_segment,
				int heredoc_fd)
{
	t_pipe_child_data		child_data;
	t_child_setup_params	setup_params;

	ft_set_child_signals();
	close_unused_heredocs(params->data, heredoc_fd);
	 if (heredoc_fd < 0 && segment_has_heredoc(params->start, params->curr))
    {
        free_token_list(cmd_segment);
        exit(130);
    }
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
	if (pid == -1)
	{
		free_token_list(cmd_segment);
		return ;
	}
	if (pid == 0)
		setup_child_process(params, cmd_segment, heredoc_fd);
	params->data->last_child_pid = pid;
	close_parent_pipes(params->data, params->i);
	free_token_list(cmd_segment);
}
