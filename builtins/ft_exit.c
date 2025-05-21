/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:18 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/21 02:26:49 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

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
	// free_shell(&g_minishell);
	// free_token_list(tk);
	if (*copy)
		free_env_list(*copy);
	exit(g_minishell.env->exit_status);
}
