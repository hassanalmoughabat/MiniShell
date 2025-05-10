/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:18 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/09 21:42:13 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

// void	ft_exit(t_token *tk, t_env **copy)
// {
// 	t_token	*curr;
// 	t_env	*curr1;
// 	char	*new_value;
// 	char	*new_line;

// 	curr = tk;
// 	curr1 = *copy;
// 	ft_putstr_fd("exit\n", 2);
// 	if (!curr)
// 		exit (g_minishell.env->exit_status);
// 	decrement(&g_minishell);
// 	// update_shlvl_in_env(g_minishell.env, g_minishell.shell_level);
// 	curr1 = g_minishell.env;
// 	while (curr1 && ft_strncmp(curr1->line, "SHLVL=", 6))
// 		curr1 = curr1->next;
// 	new_value = ft_itoa(g_minishell.shell_level);
// 	new_line = ft_strjoin("SHLVL=", new_value);
// 	if (curr1)
// 	{
// 		free(curr1->line);
// 		curr1->line = new_line;
// 	}
// 	else
// 	{
// 		add_shell_level(&g_minishell.env, new_line);
// 		free (new_line);
// 	}
// 	free (new_value);
// 	if (curr && curr->cmd)
// 	{
// 		curr = curr->next;
// 		if (!curr)
// 			g_minishell.env->exit_status = 0;
// 		else
// 			handle_exit_code(curr);
// 		// if (g_minishell.shell_level == 1)
// 		// {
// 		// 	free_token_list(tk);
// 		// 	free_env_list(*copy);
// 		// 	exit(g_minishell.env->exit_status);
// 		// }
// 		// else
// 		// 	return ;
// 	}
// 	free_token_list(tk);
// 	free_env_list(*copy);
// 	exit(g_minishell.env->exit_status);
// }

void	ft_exit(t_token *tk, t_env **copy)
{
	t_token	*curr;

	ft_putstr_fd("exit\n", 2);
	curr = tk;
	if (!curr)
		exit(g_minishell.env->exit_status);
	decrement(&g_minishell);
	update_shlvl_in_env(&g_minishell.env, g_minishell.shell_level);
	if (curr && curr->cmd)
	{
		curr = curr->next;
		if (!curr)
			g_minishell.env->exit_status = 0;
		else
			handle_exit_code(curr);
	}
	free_token_list(tk);
	free_env_list(*copy);
	exit(g_minishell.env->exit_status);
}
