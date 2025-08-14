/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:40:13 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/27 01:04:57 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	validate_delimiter(const char *delimiter)
{
	if (!delimiter || *delimiter == '\0')
	{
		return (0);
	}
	return (1);
}

int	contain_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_index(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

static int	count_var_chars(char *str, int start)
{
	int	count;

	count = 0;
	while (str[start] && str[++start] != ' ' && str[start] != '\t')
		count++;
	return (count);
}

char	*cut_from_op(char op, char *str, t_env *env, t_gc *gc)
{
	int		pos;
	int		count;
	char	*value;
	char	*send;
	int		i;

	pos = ft_index(str, op);
	count = count_var_chars(str, pos);
	value = (char *)ft_malloc(gc, sizeof(char) * (count + 1));
	if (!value)
		return (NULL);
	pos ++;
	i = 0;
	while (str[pos] && i < count)
		value[i++] = str[pos++];
	value[i] = '\0';
	send = my_getenv(value, transform(env, gc));
	return (send);
}
