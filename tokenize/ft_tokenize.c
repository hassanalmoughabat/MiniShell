/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:55:58 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/19 15:13:20 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	ft_tokenize(char *input)
{
	t_token	*head_list;

	head_list = NULL;
	ft_check_string(input, head_list);
}
