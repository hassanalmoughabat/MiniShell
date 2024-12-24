/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:58:43 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/24 21:58:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/token.h"

//ft_handle_regular_char didn't finish it yet
// should have a free and return message
void	ft_check_string(char *input, t_token **token_list)
{
	int	error_flag;

	error_flag = 0;
	while (*input)
	{
		if (error_flag)
			ft_free_token_list(token_list);
		if (ft_check_space(*input))
			ft_skip_added_spaces(&input);
		else if (ft_check_quotes(input))
			error_flag = (!ft_handle_quotes(&input, token_list));
		else if (ft_check_special_char(input))
			error_flag = (!ft_handle_operators(&input, token_list));
		else
			error_flag = (!ft_handle_regular_char(&input, token_list));
	}
}
