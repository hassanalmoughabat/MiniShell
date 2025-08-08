/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 02:35:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 02:35:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

char	*ft_handle_dollar_expansion(t_expansion_params *params)
{
	t_expand_vars	vars;

	vars.dst = &(params->dst);
	vars.result_len = params->result_len;
	vars.buffer_size = params->buffer_size;
	vars.result = params->result;
	return (ft_expand_dollar_var(params->src, params->env, &vars));
}

int	ft_handle_regular_char_expansion(t_char_expansion_params *params)
{
	t_expand_data	data;

	data.src = params->src;
	data.dst = params->dst;
	data.result_len = params->result_len;
	data.buffer_size = params->buffer_size;
	data.result = params->result;
	return (ft_expand_regular_char(&data));
}
