/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:58:47 by njoudieh          #+#    #+#             */
/*   Updated: 2025/05/02 22:46:49 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	ft_free_token_list(t_token **list)
{
	t_token	*current;
	t_token	*next;

	current = *list;
	if (!current)
		return ;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		free(current);
		current = next;
	}
	*list = NULL;
}
