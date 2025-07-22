/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:18 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 16:14:26 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_exit(t_shell *shell)
{
	t_token	*curr;

	ft_putstr_fd("exit\n", 2);
	curr = shell->tk;
	if (!curr)
		exit(shell->env->exit_status);
	decrement(&shell);
	update_shlvl_in_env(&(shell->env), shell->shell_level);
	if (curr && curr->cmd)
	{
		curr = curr->next;
		if (!curr)
			shell->env->exit_status = 0;
		else
			handle_exit_code(curr, shell);
	}
	free_env_list(shell->copy);
	exit(shell->env->exit_status);
}
