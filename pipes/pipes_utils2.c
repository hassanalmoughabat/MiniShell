/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_great_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/28 16:37:38 by njoudieh         ###   ########.fr       */
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
			t_token *curr, int i)
{
	t_token					*cmd_segment;
	t_pipe_child_data		child_data;
	t_child_setup_params	setup_params;
	pid_t					pid;
	int						heredoc_fd;

	heredoc_fd = setup_heredoc_for_command(data, start, curr, i);
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
		handle_pipe_child(&child_data);
		exit(EXIT_FAILURE);
	}
	close_parent_pipes(data, i);
	free_token_list(cmd_segment);
}

void	execute_pipe_commands(t_pipe_data *data)
{
	t_token	*curr;
	t_token	*start;
	int		i;

	start = data->lst;
	curr = next_pipe(start);
	i = 0;
	while (1)
	{
		execute_single_command(data, start, curr, i);
		if (!curr)
			break ;
		start = curr->next;
		curr = next_pipe(start);
		i++;
	}
	cleanup_heredocs_after_exec(data);
	wait_for_children(data);
}

void	setup_pipe_data(t_pipe_data *data, t_token *lst, char **ft_env,
			t_env *env)
{
	int				pipe_count;
	t_heredoc_info	*heredocs;
	int				hd_count;

	pipe_count = count_pipes(lst);
	heredocs = process_heredocs_before_pipes(lst, env, &hd_count);
	data->lst = lst;
	data->ft_env = ft_env;
	data->env = env;
	data->pipe_count = pipe_count;
	data->heredocs = heredocs;
	data->hd_count = hd_count;
}

void	handle_pipe(t_token *lst, char **ft_env, t_env *env, char *input)
{
	int			**pipes;
	t_pipe_data	data;
	int			i;

	if (!lst || !valid_pipe(lst))
		return ;
	if (has_heredoc(lst) && count_pipes(lst) == 1)
	{
		if (check_special_heredoc_pipe(lst, ft_env, env))
			return ;
	}
	setup_pipe_data(&data, lst, ft_env, env);
	if (create_pipes(&pipes, data.pipe_count) == -1)
	{
		if (data.heredocs)
		{
			i = -1;
			while (++i < data.hd_count)
				close(data.heredocs[i].fd);
			free(data.heredocs);
		}
		return ;
	}
	data.pipes = pipes;
	execute_pipe_commands(&data);
}
