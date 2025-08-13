/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:22:08 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 23:45:44 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

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
	data->last_child_pid = -1;
}

void	handle_pipe(t_token *lst, t_shell *shell, char *input)
{
	int			**pipes;
	t_pipe_data	data;
	int			i;

	if (!lst || !valid_pipe(shell, input))
		return ;
	if (has_heredoc(lst) && count_pipes(lst) == 1)
	{
		if (check_special_heredoc_pipe(lst, shell))
			return ;
	}
	setup_pipe_data(&data, lst, shell);
	if (create_pipes(&pipes, data.pipe_count) == -1)
	{
		if (data.heredocs)
		{
			i = -1;
			while (++i < data.hd_count)
				close(data.heredocs[i].fd);
			free(data.heredocs);
		}
		return ((void)(shell->env->exit_status = 1));
	}
	data.pipes = pipes;
	execute_pipe_commands(&data, shell);
}
