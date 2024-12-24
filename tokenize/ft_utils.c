/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:26:30 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/24 21:58:04 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/token.h"

//quotes = 0 has nothing to do with quotes
//quotes = -1 opened and not closed
//quotes = 2 opened and closed
t_token	*create_token(char *value, t_type_token operator)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(new_token));
	if (!new_token)
		return (0);
	new_token->type = operator;
	new_token->cmd = value;
	return (new_token);
}

void	ft_add_token_last(t_token **list, t_token *new_token)
{
	t_token	*current;

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
