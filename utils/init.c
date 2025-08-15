/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:14:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:14:55 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	get_shell_level(t_env *shell, t_gc *gc)
{
	t_env	*curr;
	char	*temp;
	char	*val;
	int		shell_val;

	curr = shell;
	while (curr && ft_strncmp(curr->line, "SHLVL=", 6))
		curr = curr->next;
	if (!curr)
		return (1);
	temp = ft_strdup_gc(gc, curr->line);
	val = ft_strchr(temp, 61);
	val ++;
	shell_val = ft_atoi(val) + 1;
	return (shell_val);
}

void	ft_add(char *key, char *value, t_env **env, t_gc *gc)
{
	t_env	*new_node1;

	new_node1 = (t_env *)ft_malloc(gc, sizeof(t_env));
	new_node1->line = quotes_in_env(value, key, 1, gc);
	new_node1->next = NULL;
	ft_push_to_env(env, new_node1);
}

void	init_empty_env(t_shell *shell)
{
	char	*pwd;

	pwd = ft_get_pwd(&shell->gc);
	if (pwd)
		ft_add("PWD", pwd, &(shell->env), &shell->gc);
	ft_add("SHLVL", "1", &(shell->env), &shell->gc);
	ft_add("_", "/usr/bin/env", &(shell->env), &shell->gc);
	shell->shell_level = 1;
}

void	ensure_pwd_exists(t_shell *shell)
{
	char	*pwd_value;
	char	*pwd;

	pwd_value = get_value_from_env("PWD", shell->env, NULL);
	if (!pwd_value || !ft_strcmp(pwd_value, ""))
	{
		pwd = ft_get_pwd(&shell->gc);
		if (pwd)
		{
			ft_add("PWD", pwd, &(shell->env), &shell->gc);
		}
	}
	free(pwd_value);
}

void	init_shell(t_shell *shell, char **envp)
{
	init_gc(&shell->gc);
	shell->env = initialize_env_list(envp, &shell->gc);
	shell->path = NULL;
	shell->tk = NULL;
	shell->curr_cmd = NULL;
	shell->copy = initialize_env_list(envp, &shell->gc);
	shell->ft_env = NULL;
	shell->key = NULL;
	shell->value = NULL;
	if (!shell->env)
		return (init_empty_env(shell));
	ensure_pwd_exists(shell);
	update_env_value(&(shell->env), "_=", "/usr/bin/env", &shell->gc);
	shell->shell_level = get_shell_level(shell->env, &shell->gc);
	update_env_value(&(shell->env), "SHLVL=",
		ft_itoa(shell->shell_level, &shell->gc), &shell->gc);
}
