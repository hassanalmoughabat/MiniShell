/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_addition.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:29 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/06 00:37:22 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

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

void	update_both(t_env **copy, t_env **env, char *key, char *value)
{
	char	*extract_key;	
	t_env	*current;
	t_env	*curr1;

	current = *env;
	curr1 = *copy;
	ft_printf("I AM HERE IN UPDATE BOTH\n");
	while (curr1 && current)
	{
		extract_key = get_var(curr1->line);
		if (!ft_strcmp(extract_key, key)
			&& ft_strlen(key) == ft_strlen(extract_key))
		{
			free(current->line);
			free(curr1->line);
			current->line = quotes_in_env(value, key, 1);
			curr1->line = quotes_in_env(value, key, 0);
			free(extract_key);
			return ;
		}
		free(extract_key);
		current = current->next;
		curr1 = curr1->next;
	}
}

void	update_copy_add_env(t_env **copy, t_env **env, char *key, char *value)
{
	char	*extract_key;
	t_env	*curr1;

	curr1 = *copy;
	ft_printf("I AM HERE IN UPDATE COPY ADD ENV FUNCTION\n");
	while (curr1)
	{
		extract_key = get_var(curr1->line);
		if (!ft_strcmp(extract_key, key) && ft_strlen(key) == ft_strlen(extract_key))
		{
			free(curr1->line);
			curr1->line = quotes_in_env(value, key, 0);
			ft_add_env(key, value, env, copy, 0);
			return ;
		}
		free(extract_key);
		curr1 = curr1->next;
	}
}

int	ft_update_env(char *key, char *value, t_env **env, t_env **copy)
{
	if (check_if_var_exist(copy, key) && check_if_var_exist(env, key))
		update_both(copy, env, key, value);
	else if (check_if_var_exist(copy, key) && !check_if_var_exist(env, key))
		update_copy_add_env(copy, env, key, value);
	return (0);
}

void	ft_add_env(char *key, char *value, t_env **env, t_env **copy, int flag)
{
	t_env	*new_node1;
	t_env	*new_node2;

	new_node1 = (t_env *)malloc(sizeof(t_env));
	if (flag)
	{
		new_node2 = (t_env *)malloc(sizeof(t_env));
		if (!new_node1 || !new_node2)
			return ;
		if (value)
		{
			new_node1->line = quotes_in_env(value, key, 1);
			new_node1->next = NULL;
			ft_push_to_env(env, new_node1);
		}
		new_node2->line = quotes_in_env(value, key, 0);
		new_node2->next = NULL;
		ft_push_to_env(copy, new_node2);
	}
	else
	{
		new_node1->line = quotes_in_env(value, key, 1);
		new_node1->next = NULL;
		ft_push_to_env(env, new_node1);
	}
}
