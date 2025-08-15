/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_extra_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:51:37 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 22:50:08 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_add_env(t_env **copy, t_shell *shell, int flag)
{
	t_env	*new_node1;
	t_env	*new_node2;

	new_node1 = (t_env *)ft_malloc(&shell->gc, sizeof(t_env));
	if (flag)
	{
		new_node2 = (t_env *)ft_malloc(&shell->gc, sizeof(t_env));
		if (!new_node1 || !new_node2)
			return ;
		if (shell->value)
		{
			new_node1->line = quotes_in_env(shell->value, shell->key, 1, &shell->gc);
			new_node1->next = NULL;
			ft_push_to_env(&(shell->env), new_node1);
		}
		new_node2->line = quotes_in_env(shell->value, shell->key, 0, &shell->gc);
		new_node2->next = NULL;
		ft_push_to_env(copy, new_node2);
	}
	else
	{
		new_node1->line = quotes_in_env(shell->value, shell->key, 1, &shell->gc);
		new_node1->next = NULL;
		ft_push_to_env(&(shell->env), new_node1);
	}
}

char	*quotes_in_env(char *value, char *key,
			int flag, t_gc *gc)
{
	char	*val;
	char	*temp;

	temp = key;
	if (value)
	{
		val = ft_strjoin(temp, "=", gc);
		if (flag)
			temp = ft_strjoin(val, value, gc);
		else
		{
			temp = ft_strjoin(val, "\"", gc);
			if (!gc)
				free(val);
			val = ft_strjoin(temp, value, gc);
			if (!gc)
				free(temp);
			temp = ft_strjoin(val, "\"", gc);
		}
		if (!gc)
			free(val);
		if (gc)
			val = ft_strdup_gc(gc, temp);
		else
			val = ft_strdup(temp);
	}
	else
	{
		if (gc)
			val = ft_strdup_gc(gc, temp);
		else
			val = ft_strdup(temp);
	}
	if (!gc && temp != key)
		free(temp);
	return (val);
}

