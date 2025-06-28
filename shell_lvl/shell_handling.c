/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:33:51 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/28 16:48:55 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	add_shell_level(t_env **env, char *new_line)
{
	t_env	*curr;
	t_env	*new_node;

	curr = *env;
	new_node = create_env_node(new_line);
	if (!new_node)
		return ;
	while (curr && ft_strncmp(curr->line, "DISPLAY=", 8))
		curr = curr->next;
	if (curr)
	{
		new_node->next = curr->next;
		curr->next = new_node;
	}
	else
	{
		new_node->next = *env;
		*env = new_node;
	}
}

void	update_shlvl_in_env(t_env **env, int new_shlvl)
{
	t_env	*curr;
	char	*new_value;
	char	*new_line;

	if (env)
		return ;
	curr = *env;
	while (curr && ft_strncmp(curr->line, "SHLVL=", 6))
		curr = curr->next;
	new_value = ft_itoa(new_shlvl);
	new_line = ft_strjoin("SHLVL=", new_value);
	free(new_value);
	if (curr)
	{
		free(curr->line);
		curr->line = new_line;
	}
	else
		add_shell_level(&g_minishell.env, new_line);
}

void	shell_handling(t_token *tk)
{
	if (tk && !ft_strncmp(tk->cmd, "./minishell", 11))
		update_shlvl_in_env(&g_minishell.env, g_minishell.shell_level);
	else
		return ;
}
