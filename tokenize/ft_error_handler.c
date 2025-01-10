/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:58:47 by njoudieh          #+#    #+#             */
/*   Updated: 2025/01/09 14:58:36 by njoudieh         ###   ########.fr       */
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
		free(current->cmd);
		free (current);
		current = next;
	}
	*list = NULL;
}

void	ft_quote_error(char c)
{
	ft_putstr_fd("Error unclosed quotes, close your ", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\n", 2);
	exit(-1);
}
