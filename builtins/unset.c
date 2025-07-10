/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:40:11 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/02 17:13:38 by njoudieh42       ###   ########.fr       */
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
			free(current->line);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	detect_key(t_token *tk, t_env **env)
{
	t_token		*curr;
	t_env		*current;
	char		*key;

	curr = tk;
	while (curr)
	{
		key = get_var(curr->cmd);
		current = *env;
		if (!ft_strcmp(curr->cmd, "_"))
			return ;
		if (key)
		{
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
		curr = curr->next;
	}
	return ;
}

void	ft_unset(t_token *token, t_env **env, t_env **copy)
{
	t_token	*current;

	if (!token || !(*env) || !env)
		return ;
	current = token;
	while (current)
	{
		if (current->cmd)
		{
			detect_key(token, env);
			detect_key(token, copy);
		}
		if (!ft_strcmp(current->cmd, "new_line"))
			return ;
		current = current->next;
	}
	g_minishell.env->exit_status = 0;
}
