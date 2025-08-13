/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:56:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 19:25:56 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

static int	resize_buffer(t_expand_data *data, int new_len)
{
	char	*new_result;
	int		offset;

	data->buffer_size = new_len * 2;
	offset = data->dst - data->result;
	new_result = realloc(data->result, data->buffer_size);
	if (!new_result)
	{
		free(data->result);
		return (0);
	}
	data->result = new_result;
	data->dst = data->result + offset;
	return (1);
}

char	*extract_var(char **src)
{
	char	var_name[256];
	int		i;

	i = 0;
	while (**src && (ft_isalnum(**src) || **src == '_') && i < 255)
	{
		var_name[i++] = **src;
		(*src)++;
	}
	var_name[i] = '\0';
	return (ft_strdup(var_name));
}

int	expand_variable(t_expand_data *data, char *var_name, t_env *env)
{
	char	*value;
	int		value_len;

	value = my_getenv(var_name, transform(env));
	if (!value)
		value = "";
	value_len = ft_strlen(value);
	if (data->result_len + value_len + 1 >= data->buffer_size)
	{
		if (!resize_buffer(data, data->result_len + value_len + 1))
			return (0);
	}
	ft_strcpy(data->dst, value);
	data->dst += value_len;
	data->result_len += value_len;
	return (1);
}

int	copy_char(t_expand_data *data, char c)
{
	if (data->result_len + 1 >= data->buffer_size)
	{
		if (!resize_buffer(data, data->buffer_size * 2))
			return (0);
	}
	*(data->dst) = c;
	data->dst++;
	data->result_len++;
	return (1);
}

int	init_expand_data(t_expand_data *data)
{
	data->result = malloc(1024);
	if (!data->result)
		return (0);
	data->dst = data->result;
	data->result_len = 0;
	data->buffer_size = 1024;
	return (1);
}
