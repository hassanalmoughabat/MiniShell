/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:26:30 by njoudieh          #+#    #+#             */
/*   Updated: 2025/05/17 14:57:05 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*create_token(char *value, t_token_type operator)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
	{
		free(value);
		return (NULL);
	}
	new_token->type = operator;
	new_token->cmd = value;
	new_token->next = NULL;
	new_token->prev = NULL;
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
