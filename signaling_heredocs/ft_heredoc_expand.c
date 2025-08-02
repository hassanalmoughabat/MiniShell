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

static char	*ft_get_var_value(char *var_name, t_env *env)
{
	char	*value;

	value = my_getenv(var_name, transform(env));
	if (!value)
		value = "";
	return (value);
}

static int	ft_resize_buffer_if_needed(t_expand_vars *vars, int value_len)
{
	if (*(vars->result_len) + value_len + 1 >= *(vars->buffer_size))
	{
		*(vars->buffer_size) = (*(vars->result_len) + value_len + 1) * 2;
		*(vars->result) = ft_realloc_buffer(*(vars->result),
				*(vars->buffer_size), vars->dst, *(vars->result_len));
		if (!*(vars->result))
			return (0);
	}
	return (1);
}

char	*ft_expand_dollar_var(char **src, t_env *env, t_expand_vars *vars)
{
	char	var_name[256];
	char	*value;
	int		i;
	int		value_len;

	(*src)++;
	ft_bzero(var_name, 256);
	i = 0;
	while (**src && (ft_isalnum(**src) || **src == '_') && i < 255)
		var_name[i++] = *(*src)++;
	var_name[i] = '\0';
	value = ft_get_var_value(var_name, env);
	value_len = ft_strlen(value);
	if (!ft_resize_buffer_if_needed(vars, value_len))
		return (NULL);
	ft_strlcpy(*(vars->dst), value, value_len + 1);
	*(vars->dst) += value_len;
	*(vars->result_len) += value_len;
	return (*(vars->result));
}

int	ft_expand_regular_char(t_expand_data *data)
{
	if (*(data->result_len) + 1 >= *(data->buffer_size))
	{
		*(data->buffer_size) *= 2;
		*(data->result) = ft_realloc_buffer(*(data->result),
				*(data->buffer_size), data->dst, *(data->result_len));
		if (!*(data->result))
			return (0);
	}
	*(*(data->dst))++ = *(*(data->src))++;
	(*(data->result_len))++;
	return (1);
}
