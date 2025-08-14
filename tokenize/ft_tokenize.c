/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:58:43 by njoudieh          #+#    #+#             */
/*   Updated: 2025/07/22 23:11:49 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*ft_check_string(char *input, t_gc *gc)
{
	t_token	*token_list;
	int		error_flag;

	token_list = NULL;
	error_flag = 0;
	while (*input)
	{
		if (error_flag)
			return (NULL);
		if (ft_check_space(*input))
			ft_skip_added_spaces(&input);
		else if (ft_check_special_char(input))
			error_flag = (!ft_handle_operators(&input, &token_list, gc));
		else
			error_flag = (!ft_handle_quotes(&input, &token_list, gc));
	}
	return (token_list);
}

t_token	*ft_tokenize(char *input, t_gc *gc)
{
	t_token	*head_list;

	head_list = ft_check_string(input, gc);
	input = NULL;
	return (head_list);
}
