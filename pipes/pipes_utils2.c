/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/09 01:29:15 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	restore_pipe_signals(void)
{
	g_signal.signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_pipe_signals(void)
{
	g_signal.signint_child = true;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	close_parent_pipes(t_pipe_data *data, int current_cmd)
{
	if (current_cmd > 0)
		close(data->pipes[current_cmd - 1][0]);
	if (current_cmd < data->pipe_count)
		close(data->pipes[current_cmd][1]);
}

void	execute_single_command(t_pipe_data *data, t_token *start,
			t_token *curr, int i, t_shell *shell)
{
	t_token					*cmd_segment;
	t_pipe_child_data		child_data;
	t_child_setup_params	setup_params;
	pid_t					pid;
	int						heredoc_fd;

	heredoc_fd = setup_heredoc_for_command(data, start, curr);
	cmd_segment = extract_command_segment(start, curr, &shell->gc);
	pid = fork();
	if (pid == 0)
	{
		ft_set_child_signals();
		close_unused_heredocs(data, heredoc_fd);
		setup_params.cmd_segment = cmd_segment;
		setup_params.i = i;
		setup_params.heredoc_fd = heredoc_fd;
		setup_child_data(&child_data, data, &setup_params);
		handle_pipe_child(&child_data, shell);
		// cmd_segment is managed by gc, no manual free needed
		// handle_pipe_child should never return - it always calls exit()
	}
	data->last_child_pid = pid;
	close_parent_pipes(data, i);
	// cmd_segment is managed by gc, no manual free needed
}

void	execute_pipe_commands(t_pipe_data *data, t_shell *shell)
{
	t_token	*curr;
	t_token	*start;
	int		i;

	setup_pipe_signals();
	start = data->lst;
	curr = next_pipe(start);
	i = 0;
	while (1)
	{
		execute_single_command(data, start, curr, i, shell);
		if (!curr)
			break ;
		start = curr->next;
		curr = next_pipe(start);
		i++;
	}
	cleanup_heredocs_after_exec(data);
	wait_for_children(data, shell);
	restore_pipe_signals();
	ft_restore_parent_signals();
}
