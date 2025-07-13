/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by njoudieh          #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	has_heredoc_before_pipe(t_token *lst)
{
	t_token	*curr;

	curr = lst;
	while (curr && curr->type != T_PIPE)
	{
		if (curr->type == T_DLESS)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	has_redirect_after_pipe(t_token *lst)
{
	t_token	*curr;

	curr = lst;
	while (curr && curr->type != T_PIPE)
		curr = curr->next;
	if (curr && curr->type == T_PIPE)
	{
		curr = curr->next;
		while (curr)
		{
			if (curr->type == T_GREAT || curr->type == T_DGREAT)
				return (1);
			curr = curr->next;
		}
	}
	return (0);
}

int	check_special_heredoc_pipe(t_token *lst, char **ft_env, t_env *env)
{
	if (has_heredoc_before_pipe(lst) && has_redirect_after_pipe(lst))
		return (handle_heredoc_pipe_redirect(lst, ft_env, env));
	return (0);
}
