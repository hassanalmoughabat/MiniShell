/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_getters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:55:30 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/25 22:13:08 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*get_value_from_env(char *key, t_env *env)
{
	char	*value;
	char	*temp;
	t_env	*current;

	current = env;
	value = NULL;
	while (current)
	{
		if (current->line && has_equal(current->line)
			&& extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
		{
			temp = ft_strchr(current->line, '=');
			temp ++;
			value = ft_strdup(temp);
			if (!value)
				return (NULL);
			return (value);
		}
		current = current->next;
	}
	return (ft_strdup(""));
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
	return (key);
}
