/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/26 17:07:55 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

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
	cmd_segment = extract_command_segment(start, curr);
	if (!cmd_segment)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close_unused_heredocs(data, heredoc_fd);
		setup_params.cmd_segment = cmd_segment;
		setup_params.i = i;
		setup_params.heredoc_fd = heredoc_fd;
		setup_child_data(&child_data, data, &setup_params);
		handle_pipe_child(&child_data, shell);
		exit(EXIT_FAILURE);
	}
	close_parent_pipes(data, i);
	free_token_list(cmd_segment);
}

void	execute_pipe_commands(t_pipe_data *data, t_shell *shell)
{
	t_token	*curr;
	t_token	*start;
	int		i;

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
	wait_for_children(data);
}

void	setup_pipe_data(t_pipe_data *data, t_token *lst, t_shell *shell)
{
	int				pipe_count;
	t_heredoc_info	*heredocs;
	int				hd_count;

	pipe_count = count_pipes(lst);
	heredocs = process_heredocs_before_pipes(lst, shell, &hd_count);
	data->lst = lst;
	data->ft_env = shell->ft_env;
	data->env = shell->env;
	data->pipe_count = pipe_count;
	data->heredocs = heredocs;
	data->hd_count = hd_count;
}

void	pipe_syntax_error(char *msg, t_shell *shell)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("'\n", 2);
	shell->env->exit_status = 2;
}
