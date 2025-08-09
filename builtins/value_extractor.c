/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_extractor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:39:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/06 15:51:56 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*extract_dollar_var(char *key, int *index)
{
	char	*dollar_value;
	int		start;
	int		length;
	int		j;

	start = *index;
	length = 0;
	j = 0;
	while (key[*index] && (ft_isalnum(key[*index]) || key[*index] == '_'))
	{
		(*index)++;
		length++;
	}
	dollar_value = malloc(sizeof(char) * (length + 1));
	if (!dollar_value)
		return (NULL);
	while (j < length)
	{
		dollar_value[j] = key[start + j];
		j++;
	}
	dollar_value[j] = '\0';
	return (dollar_value);
}

char	*extract_quoted_substring(char *input, int *i)
{
	char	quote;
	int		start;
	char	*substr;

	quote = input[*i];
	start = (*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	substr = ft_substr(input, start, (*i) - start);
	return (substr);
}

char	*extract_unquoted_substring(char *input, int *i, int *flag)
{
	int		start;
	int		len;
	char	*substr;

	start = *i;
	len = 0;
	while (input[*i])
	{
		if (input[*i] == '$' && !escape(input, *i))
			*flag = 1;
		(*i)++;
		len ++;
	}
	substr = ft_substr(input, start, len);
	return (substr);
}

size_t	extract_key_env(char *line)
{
	size_t	len;

	len = 0;
	while (line[len] && line[len] != '=')
		len ++;
	return (len);
}
