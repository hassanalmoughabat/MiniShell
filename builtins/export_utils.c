/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:34:50 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 14:20:35 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp;

	temp = a->line;
	a->line = b->line;
	b->line = temp;
}

void	sort_env_list(t_env *env)
{
	int		swapped;
	t_env	*ptr;
	t_env	*last;

	last = NULL;
	swapped = 1;
	if (!env || !env)
		return ;
	while (swapped)
	{
		swapped = 0;
		ptr = env;
		while (ptr->next != last)
		{
			if (ft_strcmp(ptr->line, ptr->next->line) > 0)
			{
				swap_env_nodes(ptr, ptr->next);
				swapped = 1;
			}
			ptr = ptr->next;
		}
		last = ptr;
	}
}

int	print_export_env( t_shell *shell, int flag, t_env *copy)
{
	t_token	*curr;

	curr = shell->tk;
	if ((ft_list_size(shell->tk) == 1 && curr->cmd
			&& !ft_strcmp(curr->cmd, "export"))
		|| flag)
	{
		sort_env_list(copy);
		while (copy)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(copy->line, 1);
			ft_putstr_fd("\n", 1);
			copy = copy->next;
		}
		shell->env->exit_status = ENU_SUCCESS;
		return (1);
	}
	return (0);
}

int	check_if_var_exist(t_env **env, char *key)
{
	t_env	*current;

	if (!env || !*env || !key)
		return (0);
	current = *env;
	while (current)
	{
		if (has_equal(current->line) && current->line
			&& extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
			return (1);
		if (!has_equal(current->line) && current->line
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
			return (1);
		current = current->next;
	}
	return (0);
}
