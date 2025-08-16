/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_segment_redirect_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 16:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	setup_input_redirection(t_pipe_child_data *child_data)
{
	t_token	*curr;
	int		fd;
	char	*filename;

	curr = child_data->cmd_segment;
	while (curr)
	{
		if (curr->type == T_LESS && curr->next)
		{
			filename = curr->next->cmd;
			fd = open(filename, O_RDONLY);
			if (fd >= 0)
				dup2(fd, STDIN_FILENO);
			return ;
		}
		curr = curr->next;
	}
}

void	setup_output_redirection(t_pipe_child_data *child_data)
{
	t_token	*curr;

	curr = child_data->cmd_segment;
	while (curr)
	{
		if (curr->type == T_GREAT && curr->next)
		{
			handle_great_redirect(curr);
			return ;
		}
		else if (curr->type == T_DGREAT && curr->next)
		{
			handle_dgreat_redirect(curr);
			return ;
		}
		curr = curr->next;
	}
}

t_token	*remove_redirection_tokens(t_token *cmd_segment)
{
	t_token	*curr;
	t_token	*next;

	curr = cmd_segment;
	while (curr)
	{
		next = curr->next;
		if (curr->type == T_GREAT || curr->type == T_DGREAT
			|| curr->type == T_LESS || curr->type == T_DLESS)
		{
			if (curr->next && curr->next->type == T_IDENTIFIER)
			{
				if (curr->prev)
					curr->prev->next = curr->next->next;
				else
					cmd_segment = curr->next->next;
				if (curr->next->next)
					curr->next->next->prev = curr->prev;
				next = curr->next->next;
			}
		}
		curr = next;
	}
	return (cmd_segment);
}
