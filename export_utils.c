/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:34:50 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/07 04:18:02 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

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
	if (!env || !env)
		return ;
	while (swapped)
	{
		swapped = 0;
		ptr = env;
		while (ptr->next != last)
		{
			if (strcmp(ptr->line, ptr->next->line) > 0)
			{
				swap_env_nodes(ptr, ptr->next);
				swapped = 1;
			}
			ptr = ptr->next;
		}
		last = ptr;
	}
}

int	print_export_env( t_token *token, t_env *env)
{
	t_token	*curr;

	curr = token;
	if (ft_list_size(token) == 1 && curr->cmd
		&& !ft_strcmp(curr->cmd, "export"))
	{
		// sort_env_list(env);export
		while (env)
		{
			printf("declare -x %s\n", env->line);
			env = env->next;
		}
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
