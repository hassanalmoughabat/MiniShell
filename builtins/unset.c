/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:40:11 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 13:08:16 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	delete_env_node(t_env **env, char *target)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !target)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->line
			&& !ft_strncmp(current->line, target, ft_strlen(target)))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	remove_matching_env(t_env **env, char *key)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->line, "_=/usr/bin/env") == 0)
		{
			current = current->next;
			continue ;
		}
		if (extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
		{
			delete_env_node(env, key);
			break ;
		}
		current = current->next;
	}
}

void	detect_key(t_token *tk, t_env **env, t_gc *gc)
{
	t_token		*curr;
	char		*key;

	curr = tk;
	while (curr)
	{
		key = get_var(curr->cmd, gc);
		if (!ft_strcmp(curr->cmd, "_"))
			return ;
		if (key)
			remove_matching_env(env, key);
		curr = curr->next;
	}
	return ;
}

void	ft_unset(t_shell *shell, t_env **copy)
{
	t_token	*current;

	if (!shell->tk || !shell->env || !*copy)
		return ;
	current = shell->tk;
	while (current)
	{
		if (current->cmd)
		{
			detect_key(shell->tk, &(shell->env), &shell->gc);
			detect_key(shell->tk, copy, &shell->gc);
		}
		if (!ft_strcmp(current->cmd, "new_line"))
			return ;
		current = current->next;
	}
	shell->env->exit_status = 0;
}
