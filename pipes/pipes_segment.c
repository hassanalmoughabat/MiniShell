/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_segment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 10:43:34 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	ft_pipe_builtin(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if ((ft_strcmp(curr->cmd, "cd") == 0))
			return (1);
		else if (ft_strcmp(curr->cmd, "echo") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "unset") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "env") == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

t_token	*create_token_copy(t_token *src, t_gc *gc)
{
	t_token	*new_token;

	new_token = ft_malloc(gc, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->cmd = ft_strdup_gc(gc, src->cmd);
	new_token->type = src->type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	add_token_to_list(t_token **new_list, t_token **last,
			t_token *new_token)
{
	new_token->prev = *last;
	if (!*new_list)
		*new_list = new_token;
	else
		(*last)->next = new_token;
	*last = new_token;
}

void	remove_heredoc_tokens(t_pipe_child_data *child_data)
{
	t_token	*cur;
	t_token	*next;

	cur = child_data->cmd_segment;
	while (cur)
	{
		if (cur->type == T_DLESS && cur->next)
		{
			next = cur->next->next;
			if (cur->prev)
				cur->prev->next = next;
			else
				child_data->cmd_segment = next;
			if (next)
				next->prev = cur->prev;
			cur = next;
			continue ;
		}
		cur = cur->next;
	}
}
