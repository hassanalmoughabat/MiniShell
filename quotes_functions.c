/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:35:27 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/07 01:38:17 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

bool	contains_mixed_quotes(char *str)
{
	bool	in_single = false;
	bool	in_double = false;
	bool	found_single = false;
	bool	found_double = false;
	int		i = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			if (!in_single)
				found_single = true;
		}
		else if (str[i] == '"' && !in_single && !escape(str, i))
		{
			in_double = !in_double;
			if (!in_double)
				found_double = true;
		}
		if (found_single && found_double)
			return true;
		i++;
	}
	return false;
}

int quote_type(char *str)
{
    bool in_single = false;
    bool in_double = false;
    bool found_single = false;
    bool found_double = false;
    int i = 0;

    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
        {
            in_single = !in_single;
            if (!in_single)
                found_single = true;
        }
        else if (str[i] == '"' && !in_single && !escape(str, i))
        {
            in_double = !in_double;
            if (!in_double)
                found_double = true;
        }
        i++;
    }
    if (found_single && found_double)
        return 3;
    if (found_double)
        return 2;
    if (found_single)
        return 1;
    return 0;
}

int	remove_added_quotes(char **value)
{
	char	*temp;
	char	*result;
	size_t	i;
	size_t	j;

	if (!*value || !value)
		return (0);
	temp = *value;
	result = malloc(ft_strlen(temp) + 1);
	i = 0;
	j = 0;
	if (!result)
		return (-1);
	while (i < ft_strlen(temp) && temp[i])
	{
		if (ft_check_quotes(temp[i]) && !escape(temp, i))
		{
			if (handle_export_quotes(temp, result, &i, &j) == -1)
				return (free(result), -1);
		}
		else
			result[j++] = temp[i++];
	}
	result[j] = '\0';
	if (*value)
		free (*value);
	*value = ft_strdup(result);
	return (0);
}
