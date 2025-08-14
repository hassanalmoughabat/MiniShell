/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 22:54:20 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/25 13:19:28 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

static int	handle_dash_n(char *normalized, char **result, int *i, t_gc *gc)
{
	int	start;

	start = *i;
	(*i)++;
	while (normalized[*i] && normalized[*i] == 'n')
		(*i)++;
	if (normalized[*i] != ' ' && normalized[*i] != '\0')
	{
		*result = ft_strdup_gc(gc, normalized + start);
		return (1);
	}
	if (normalized[*i] == ' ')
		(*i)++;
	return (0);
}

int	extract_word(char *normalized, char **result, t_gc *gc)
{
	int		i;

	i = 0;
	while (normalized[i])
	{
		if (normalized[i] == '-')
		{
			if (handle_dash_n(normalized, result, &i, gc))
				return (1);
		}
		else
		{
			*result = ft_strdup_gc(gc, normalized + i);
			return (1);
		}
	}
	return (0);
}

char	*extract_content_after_flags(char *str, t_gc *gc)
{
	char	*temp;
	char	*normalized;
	char	*result;

	temp = ft_strdup_gc(gc, str);
	if (!temp)
		return (NULL);
	ft_skip_added_spaces(&temp);
	if (!is_quote_token(temp))
		remove_added_quotes(&temp, gc);
	normalized = normalize_spaces(temp, gc);
	if (!normalized)
		return (NULL);
	extract_word(normalized, &result, gc);
	return (result);
}

int	check_flag(char *temp, int *has_flags, int *has_content)
{
	int	i;
	int	start;

	i = 0;
	while (temp[i])
	{
		if (temp[i] == '-')
		{
			start = i++;
			while (temp[i] && temp[i] == 'n')
				i++;
			if ((temp[i] == ' ' || temp[i] == '\0') && i > start + 1)
			{
				*has_flags = 1;
				while (temp[i] == ' ')
					i++;
			}
			else
				return (*has_content = 1, 0);
		}
		else
			return (*has_content = 1, 0);
	}
	return (0);
}

int	check_n_flags(char *str, t_gc *gc)
{
	char	*temp;
	int		has_flags;
	int		has_content;

	has_flags = 0;
	has_content = 0;
	if (!str)
		return (0);
	temp = ft_strdup_gc(gc, str);
	ft_skip_added_spaces(&temp);
	if (!is_quote_token(temp))
		remove_added_quotes(&temp, gc);
	check_flag(temp, &has_flags, &has_content);
	if (has_flags)
	{
		if (has_content)
			return (2);
		return (1);
	}
	return (0);
}
