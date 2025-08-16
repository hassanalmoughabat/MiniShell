/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc_util2_extra.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 17:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	setup_heredoc_for_command(t_pipe_data *data, t_token *start,
		t_token *curr)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	if (data->heredocs && segment_has_heredoc(start, curr))
	{
		heredoc_fd = get_heredoc_fd_for_segment(start, curr,
				data->heredocs, data->hd_count);
	}
	return (heredoc_fd);
}

void	close_unused_heredocs(t_pipe_data *data, int heredoc_fd)
{
	int	j;

	if (!data->heredocs)
		return ;
	j = 0;
	while (j < data->hd_count)
	{
		if (data->heredocs[j].fd != heredoc_fd
			&& data->heredocs[j].fd >= 0)
			close(data->heredocs[j].fd);
		j++;
	}
}

void	setup_child_data(t_pipe_child_data *child_data, t_pipe_data *data,
		t_child_setup_params *params)
{
	child_data->cmd_segment = params->cmd_segment;
	child_data->ft_env = data->ft_env;
	child_data->env = data->env;
	child_data->i = params->i;
	child_data->pipes = data->pipes;
	child_data->pipe_count = data->pipe_count;
	child_data->heredoc_fd = params->heredoc_fd;
	child_data->is_first_with_heredoc = (params->i == 0
			&& params->heredoc_fd >= 0);
}
