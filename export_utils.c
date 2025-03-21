/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:34:50 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 03:04:51 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	check_if_var_exist(t_env **env, char *key)
{
	t_env	*current;

	if (!env || !*env || !key)
		return (0);
	current = *env;
	while (current)
	{
		
		if (current->line
			&& extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
			return (1);
		current = current->next;
	}
	return (0);
}

int	remove_added_quotes(char **value, int flag)
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
		if (ft_check_quotes(temp[i]))
		{
			if (handle_export_quotes(temp, result, &i, &j, flag) == -1)
				return (-1);	
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

void	ft_push_to_env(t_env **env, t_env *node)
{
	t_env	*curr;

	if (!env || !node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	curr = *env;
	while (curr->next)
		curr = curr->next;
	curr->next = node;
}
