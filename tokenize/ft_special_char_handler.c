/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_special_char_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:43:38 by njoudieh          #+#    #+#             */
/*   Updated: 2025/05/16 14:18:58 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	ft_append_operator(t_token **list, t_token_type operator,
	char **input, char *value)
{
	t_token	*new_token;

	new_token = create_token(value, operator);
	if (!new_token)
		return (0);
	ft_add_token_last(list, new_token);
	(*input)++;
	if (operator == T_DLESS || operator == T_DGREAT)
		(*input)++;
	return (1);
}

int	ft_check_special_char(char *str)
{
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1) || !ft_strncmp(str, "\n", 1))
		return (1);
	return (0);
}

int	ft_handle_operators(char **input, t_token **list)
{
	if (**input == '>' && (*input)[1] != '>')
		return (ft_append_operator(list, T_GREAT, input, ">"));
	else if (**input == '<' && (*input)[1] != '<')
		return (ft_append_operator(list, T_LESS, input, "<"));
	else if (**input == '>' && (*input)[1] == '>')
		return (ft_append_operator(list, T_DGREAT, input, ">>"));
	else if (**input == '<' && (*input)[1] == '<')
		return (ft_append_operator(list, T_DLESS, input, "<<"));
	else if (!ft_strncmp(*input, "|", 1))
		return (ft_append_operator(list, T_PIPE, input, "|"));
	else if (!ft_strncmp(*input, "\n", 1))
		return (ft_append_operator(list, T_NL, input, "new_line"));
	return (0);
}
