/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:58:43 by njoudieh          #+#    #+#             */
/*   Updated: 2025/01/10 15:59:00 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*ft_check_string(char *input)
{
	t_token	*token_list;
	int		error_flag;

	token_list = NULL;
	error_flag = 0;
	while (*input)
	{
		if (error_flag)
			return (ft_free_token_list(&token_list), NULL);
		if (ft_check_space(*input))
			ft_skip_added_spaces(&input);
		else if (ft_check_special_char(input))
			error_flag = (!ft_handle_operators(&input, &token_list));
		else
			error_flag = (!ft_handle_quotes(&input, &token_list));
	}
	return (token_list);
}

t_token	*eft_tokenize(char *input)
{
	t_token	*head_list;

	head_list = ft_check_string(input);
	input = NULL;
	return (head_list);
}
