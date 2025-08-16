/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_addition.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:29 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 22:44:23 by njoudieh42       ###   ########.fr       */
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

void	ft_add_key_to_env(t_env **copy, char *key, t_gc *gc)
{
	t_env	*new_node;

	if (!key)
		return ;
	if (gc)
	{
		new_node = (t_env *)ft_malloc(gc, sizeof(t_env));
		if (!new_node)
			return ;
		new_node->line = ft_strdup_gc(gc, key);
	}
	else
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->line = ft_strdup(key);
	}
	new_node->next = NULL;
	ft_push_to_env(copy, new_node);
}

void	update_value(t_update_params *params)
{
	char	*extract_key;	
	t_env	*current;

	current = *params->env;
	while (current)
	{
		extract_key = get_var(current->line, params->gc);
		if (!ft_strcmp(extract_key, params->key)
			&& ft_strlen(params->key) == ft_strlen(extract_key))
		{
			current->line = quotes_in_env(params->value, params->key,
					params->flag, NULL);
			return ;
		}
		current = current->next;
	}
}

void	update_copy_add_env(t_shell *shell, t_env **copy)
{
	char	*extract_key;
	t_env	*curr1;

	curr1 = *copy;
	while (curr1)
	{
		extract_key = get_var(curr1->line, &shell->gc);
		if (!ft_strcmp(extract_key, shell->key)
			&& ft_strlen(shell->key) == ft_strlen(extract_key))
		{
			curr1->line = quotes_in_env(shell->value, shell->key, 0, NULL);
			ft_add_env(copy, shell, 0);
			return ;
		}
		curr1 = curr1->next;
	}
}

int	ft_update_env(t_env **copy, t_shell *shell)
{
	t_update_params	uparams1;
	t_update_params	uparams2;

	if (check_if_var_exist(copy, shell->key)
		&& check_if_var_exist(&(shell->env), shell->key))
	{
		uparams1.env = copy;
		uparams1.key = shell->key;
		uparams1.value = shell->value;
		uparams1.flag = 0;
		uparams1.gc = &shell->gc;
		update_value(&uparams1);
		uparams2.env = &(shell->env);
		uparams2.key = shell->key;
		uparams2.value = shell->value;
		uparams2.flag = 1;
		uparams2.gc = &shell->gc;
		update_value(&uparams2);
	}
	else if (check_if_var_exist(copy, shell->key)
		&& !check_if_var_exist(&(shell->env), shell->key))
		update_copy_add_env(shell, copy);
	return (0);
}
