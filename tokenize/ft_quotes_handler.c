/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:06:54 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/19 15:16:57 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	check_unique_char(char s, char before)
{
	if (s == '\\' && before == '"')
		return (1);
	return (0);
}

int	ft_check_quotes(char *input, t_token **list)
{
	
}

//if (quote == 1) FOR HERE DOC check if even valid(closed quoted)
//and odd invalid heredoc implentation
//handle echo quotes
//handle the different cases between unclosed quotes and no quotes at all