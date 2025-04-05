/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_getters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:55:30 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/05 18:49:05 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

char	*get_value_from_env(char *key, t_env *env)
{
	char	*value;
	t_env	*current;
	int		i;
	int		len;

	i = 0;
	current = env;
	value = NULL;
	while (current)
	{
		if (current->line && extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
		{
			while (current->line && current->line[i] != '=')
				i ++;
			len = ft_strlen(current->line) - i -1;
			value = ft_substr(current->line, i + 1, len);
			if (remove_added_quotes(&value) == -1)
				return (free(value), NULL);
			return (value);
		}
		current = current->next;
	}
	return (value);
}

char	*get_var(char *input)
{
	size_t	i;
	char	*key;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i] && input[i] != '=')
		i ++;
	key = ft_substr(input, 0, i);
	if (!check_valid_key(key))
		return (NULL);
	return (key);
}
