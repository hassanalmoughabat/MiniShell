/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:33:35 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/09 12:25:03 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*ft_strcat(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*ptr)
		ptr++;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}

char	*join_env_value(char *expanded, char *value)
{
	char	*result;

	if (!expanded && !value)
		result = ft_strdup("");
	else if (!expanded)
		result = ft_strdup(value);
	else if (!value)
		result = ft_strdup(expanded);
	else
		result = ft_strjoin(expanded, value);
	return (result);
}

int	ft_check_dollar(char *value, int index)
{
	if (value[index] == '$' && !escape(value, index))
		return (1);
	return (0);
}

int	ft_has_dollar(char *str)
{
	int		index;

	index = 0;
	while (str[index])
	{
		if (str[index] == '$' && !escape(str, index))
			return (1);
		index ++;
	}
	return (0);
}
