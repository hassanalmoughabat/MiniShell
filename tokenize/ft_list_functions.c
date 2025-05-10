/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:26:30 by njoudieh          #+#    #+#             */
/*   Updated: 2025/05/07 20:31:36 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*create_token(char *value, t_token_type operator)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = operator;
	if (value)
	{
		new_token->cmd = ft_strdup(value);
		if (!new_token->cmd)
		{
			free(new_token);
			return (NULL);
		}
	}
	else
		new_token->cmd = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	ft_add_token_last(t_token **list, t_token *new_token)
{
	t_token	*current;

	if (!list || !new_token)
		return ;
	current = *list;
	if (!(*list))
	{
		*list = new_token;
		return ;
	}
	while (current->next)
		current = current ->next;
	current->next = new_token;
	new_token->prev = current;
}
