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

int	ft_append_operator(t_append_params *params)
{
	t_token	*new_token;

	new_token = create_token(params->value, params->operator, params->gc);
	if (!new_token)
		return (0);
	ft_add_token_last(params->list, new_token);
	(*params->input)++;
	if (params->operator == T_DLESS || params->operator == T_DGREAT)
		(*params->input)++;
	return (1);
}

int	ft_check_special_char(char *str)
{
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1) || !ft_strncmp(str, "\n", 1))
		return (1);
	return (0);
}

static void	init_params(t_append_params *params, t_token **list,
				char **input, t_gc *gc)
{
	params->list = list;
	params->input = input;
	params->gc = gc;
}

int	ft_handle_operators(char **input, t_token **list, t_gc *gc)
{
	t_append_params	params;

	init_params(&params, list, input, gc);
	if (**input == '>' && (*input)[1] != '>')
		return (params.operator = T_GREAT, params.value = ">",
			ft_append_operator(&params));
	else if (**input == '<' && (*input)[1] != '<')
		return (params.operator = T_LESS, params.value = "<",
			ft_append_operator(&params));
	else if (**input == '>' && (*input)[1] == '>')
		return (params.operator = T_DGREAT, params.value = ">>",
			ft_append_operator(&params));
	else if (**input == '<' && (*input)[1] == '<')
		return (params.operator = T_DLESS, params.value = "<<",
			ft_append_operator(&params));
	else if (!ft_strncmp(*input, "|", 1))
		return (params.operator = T_PIPE, params.value = "|",
			ft_append_operator(&params));
	else if (!ft_strncmp(*input, "\n", 1))
		return (params.operator = T_NL, params.value = "new_line",
			ft_append_operator(&params));
	return (0);
}
