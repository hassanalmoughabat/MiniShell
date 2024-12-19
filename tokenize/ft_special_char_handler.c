/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_special_char_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:43:38 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/18 15:47:38 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	ft_append_operator(t_token **list, t_type_token operator, char **input)
{
	t_token	*new_token;

	new_token = create_token(NULL, operator);
	if (!new_token)
		return (0);
	ft_add_token_last(list, new_token);
	(*input)++;
	if (operator == '>' || operator == '>')
		(*input)++;
	return (1);
}

//check if there is a special char 
int	ft_check_special_char(char *str)
{
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1))
		return (1);
	return (0);
}

int	ft_handle_operators(char *input, t_token **list)
{
	if (!ft_strncmp(input, "<", 1))
		return (ft_append_operator(list, T_LESS, input));
	else if (!ft_strncmp(input, ">", 1))
		return (ft_append_operator(list, T_GREAT, input));
	else if (!ft_strncmp(input, "<<", 1))
		return (ft_append_operator(list, T_DLESS, input));
	else if (!ft_strncmp(input, ">>", 1))
		return (ft_append_operator(list, T_DGREAT, input));
	else if (!ft_strncmp(input, "|", 1))
		return (ft_append_operator(list, T_PIPE, input));
}
