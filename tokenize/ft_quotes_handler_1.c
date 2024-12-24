/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_handler_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:28:39 by njoudieh42        #+#    #+#             */
/*   Updated: 2024/12/24 21:57:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/token.h"

int	ft_handle_quotes(char **input, t_token **list)
{
	t_token	*new_token;
	char	*temp;
	int		*i;
	int		*j;

	i = 0;
	j = 0;
	while (j <= ft_strlen(*input) && **input)
	{
		if (ft_check_type_quote(**input))
		{
			if (ft_check_type_quote(**input) == T_Squote)
				temp = ft_handle_single_quotes(input, &i, &j);
			else
				temp = ft_handle_double_quotes(input, &i, &j);
			new_token = create_token(temp, ft_check_type_quote(**input));
		}
		else if (ft_check_special_char(*input))
			return (0);
		else
		{
			temp = ft_handle_word(input, &i, &j);
			new_token = create_token(temp, T_IDENTIFIER);
		}
		ft_add_token_last(list, new_token);
	}
	free(temp);
	return (1);
}
