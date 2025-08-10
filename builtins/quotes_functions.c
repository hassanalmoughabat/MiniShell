/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:35:27 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/25 13:19:05 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	bool_set(bool *quotes, bool *found)
{
	*quotes = !*quotes;
	if (!*quotes)
		*found = true;
}

int	quote_type(char *str)
{
	bool	in_single;
	bool	in_double;
	bool	found_single;
	bool	found_double;
	int		i;

	i = 0;
	in_double = false;
	in_single = false;
	found_double = false;
	found_single = false;
	while (str[i++])
	{
		if (str[i] == '\'' && !escape(str, i) && !in_double)
			bool_set(&in_single, &found_single);
		else if (str[i] == '"' && !in_single && !escape(str, i))
			bool_set(&in_double, &found_double);
	}
	if (found_single && found_double)
		return (3);
	if (found_double)
		return (2);
	if (found_single)
		return (1);
	return (0);
}

int	remove_added_quotes(char **value)
{
	char	*temp;
	char	*result;
	size_t	i;
	size_t	j;

	if (!value || !*value)
		return (0);
	temp = *value;
	result = malloc(ft_strlen(temp) + 1);
	if (!result)
		return (1);
	i = 0;
	j = 0;
	while (i < ft_strlen(temp) && temp[i])
	{
		if (ft_check_quotes(temp[i]) && !escape(temp, i)
			&& handle_export_quotes(temp, result, &i, &j) == -1)
			return (free(result), -1);
		else if (!ft_check_quotes(temp[i]) || escape(temp, i))
			result[j++] = temp[i++];
	}
	result[j] = '\0';
	free(temp);
	*value = ft_strdup(result);
	return (free(result), 0);
}
