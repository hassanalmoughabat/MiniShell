/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/15 01:03:34 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	setup_pipe_redirects(t_pipe_redirect_params *params)
{
	// (void)params->is_first_with_heredoc;
	if (params->heredoc_fd >= 0)
	{
		if (dup2(params->heredoc_fd, STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(params->heredoc_fd);
	}
	else if (params->i > 0)
	{
		if (dup2(params->pipes[params->i - 1][0], STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
	}
	if (params->i < params->pipe_count)
	{
		if (dup2(params->pipes[params->i][1], STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
	}
	close_all_pipes(params->pipes, params->pipe_count);
}

t_token	*extract_command_segment(t_token *start, t_token *end)
{
	t_token	*new_list;
	t_token	*curr;
	t_token	*new_token;
	t_token	*last;

	new_list = NULL;
	last = NULL;
	curr = start;
	while (curr && curr != end)
	{
		new_token = create_token_copy(curr);
		if (!new_token)
		{
			free_token_list(new_list);
			return (NULL);
		}
		add_token_to_list(&new_list, &last, new_token);
		curr = curr->next;
	}
	return (new_list);
}

void	handle_pipe_child(t_pipe_child_data *child_data, t_shell *shell)
{
	t_pipe_redirect_params	params;

	params.i = child_data->i;
	params.pipes = child_data->pipes;
	params.pipe_count = child_data->pipe_count;
	params.heredoc_fd = child_data->heredoc_fd;
	params.is_first_with_heredoc = child_data->is_first_with_heredoc;
	setup_pipe_redirects(&params);
	remove_heredoc_tokens(child_data);
	handle_redirections_and_execute(child_data, shell);
}

void	close_parent_heredoc(t_pipe_data *data, int heredoc_fd)
{
	int	j;

	if (heredoc_fd < 0)
		return ;
	j = 0;
	while (j < data->hd_count)
	{
		if (data->heredocs[j].fd == heredoc_fd)
		{
			close(data->heredocs[j].fd);
			data->heredocs[j].fd = -1;
			break ;
		}
		j++;
	}
}
