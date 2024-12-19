/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:58:47 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/19 14:32:05 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	ft_free_token_list(t_token **list)
{
	t_token	*current;

	while (current)
	{
		current = *list;
		list = (*list)->next;
		free (current);
	}
}
