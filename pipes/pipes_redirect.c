/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by njoudieh          #+#    #+#             */
/*   Updated: 2025/08/14 18:59:16 by njoudieh42       ###   ########.fr       */
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

int	check_special_heredoc_pipe(t_token *lst, t_shell *shell)
{
	int	result;

	if (has_heredoc_before_pipe(lst) && has_redirect_after_pipe(lst))
	{
		result = handle_heredoc_pipe_redirect(lst, shell);
		if (shell->env->exit_status == 130)
			return (2);
		return (result);
	}
	return (0);
}
