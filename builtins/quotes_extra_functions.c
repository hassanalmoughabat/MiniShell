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

static void	handle_flag_case(t_env **copy, t_shell *shell, t_env *new_node1)
{
	t_env	*new_node2;

	new_node2 = (t_env *)ft_malloc(&shell->gc, sizeof(t_env));
	if (!new_node1 || !new_node2)
		return ;
	if (shell->value)
	{
		new_node1->line = quotes_in_env(shell->value, shell->key, 1,
				&shell->gc);
		new_node1->next = NULL;
		ft_push_to_env(&(shell->env), new_node1);
	}
	new_node2->line = quotes_in_env(shell->value, shell->key, 0,
			&shell->gc);
	new_node2->next = NULL;
	ft_push_to_env(copy, new_node2);
}

void	ft_add_env(t_env **copy, t_shell *shell, int flag)
{
	t_env	*new_node1;

	new_node1 = (t_env *)ft_malloc(&shell->gc, sizeof(t_env));
	if (flag)
		handle_flag_case(copy, shell, new_node1);
	else
	{
		new_node1->line = quotes_in_env(shell->value, shell->key, 1,
				&shell->gc);
		new_node1->next = NULL;
		ft_push_to_env(&(shell->env), new_node1);
	}
}

static char	*create_quoted_value(char *val, char *value, t_gc *gc)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(val, "\"", gc);
	if (!gc)
		free(val);
	val = ft_strjoin(temp, value, gc);
	if (!gc)
		free(temp);
	result = ft_strjoin(val, "\"", gc);
	if (!gc)
		free(val);
	return (result);
}

static char	*handle_value_case(char *key, char *value, int flag, t_gc *gc)
{
	char	*val;
	char	*temp;

	val = ft_strjoin(key, "=", gc);
	if (flag)
		temp = ft_strjoin(val, value, gc);
	else
		temp = create_quoted_value(val, value, gc);
	if (!gc)
		free(val);
	if (gc)
		return (ft_strdup_gc(gc, temp));
	else
		return (ft_strdup(temp));
}

char	*quotes_in_env(char *value, char *key, int flag, t_gc *gc)
{
	char	*val;
	char	*temp;

	if (value)
	{
		temp = handle_value_case(key, value, flag, gc);
		if (!gc && temp != key)
			free(temp);
		return (temp);
	}
	else
	{
		if (gc)
			val = ft_strdup_gc(gc, key);
		else
			val = ft_strdup(key);
		return (val);
	}
}
