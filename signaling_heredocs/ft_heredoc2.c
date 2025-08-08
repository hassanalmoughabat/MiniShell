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

static char	*find_env_value(char *var_name, t_env *env)
{
	t_env	*current;
	char	*equals_pos;
	int		name_len;

	name_len = ft_strlen(var_name);
	current = env;
	while (current)
	{
		if (current->line && ft_strncmp(current->line, var_name, name_len) == 0)
		{
			equals_pos = ft_strchr(current->line, '=');
			if (equals_pos && equals_pos == current->line + name_len)
				return (equals_pos + 1);
		}
		current = current->next;
	}
	return (NULL);
}
