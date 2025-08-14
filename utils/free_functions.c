/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:50:35 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/27 01:55:35 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->cmd);
		free(head);
		head = tmp;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current != NULL)
	{
		next = current->next;
		if (current->line)
			free(current->line);
		free(current);
		current = next;
	}
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	// Environment cleanup handled by garbage collector
	// if (shell->env)
	//	free_env_list(shell->env);
	if (shell->path)
		free(shell->path);
	free(shell);
}
