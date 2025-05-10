/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_addition.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:29 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/11 00:12:48 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

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
	return ;
}

void	ft_add_key_to_env(t_env **copy, char *key)
{
	t_env	*new_node;

	if (!key)
		return ;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->line = ft_strdup(key);
	new_node->next = NULL;
	ft_push_to_env(copy, new_node);
}

void	update_value(t_env **env, char *key, char *value, int flag)
{
	char	*extract_key;	
	t_env	*current;

	current = *env;
	while (current)
	{
		extract_key = get_var(current->line);
		if (!ft_strcmp(extract_key, key)
			&& ft_strlen(key) == ft_strlen(extract_key))
		{
			free(current->line);
			current->line = quotes_in_env(value, key, flag);
			free(extract_key);
			return ;
		}
		free(extract_key);
		current = current->next;
	}
}

void	update_copy_add_env(t_env **copy, char *key, char *value)
{
	char	*extract_key;
	t_env	*curr1;

	curr1 = *copy;
	while (curr1)
	{
		extract_key = get_var(curr1->line);
		if (!ft_strcmp(extract_key, key)
			&& ft_strlen(key) == ft_strlen(extract_key))
		{
			free(curr1->line);
			curr1->line = quotes_in_env(value, key, 0);
			ft_add_env(key, value, copy, 0);
			return ;
		}
		free(extract_key);
		curr1 = curr1->next;
	}
}

int	ft_update_env(char *key, char *value, t_env **env, t_env **copy)
{
	if (check_if_var_exist(copy, key) && check_if_var_exist(env, key))
	{
		update_value(copy, key, value, 0);
		update_value(env, key, value, 1);
	}
	else if (check_if_var_exist(copy, key) && !check_if_var_exist(env, key))
		update_copy_add_env(copy, key, value);
	return (0);
}
