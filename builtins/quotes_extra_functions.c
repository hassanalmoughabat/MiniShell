/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_extra_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:51:37 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 14:07:15 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_add_env(t_env **copy, t_shell *shell, int flag)
{
	t_env	*new_node1;
	t_env	*new_node2;

	new_node1 = (t_env *)malloc(sizeof(t_env));
	if (flag)
	{
		new_node2 = (t_env *)malloc(sizeof(t_env));
		if (!new_node1 || !new_node2)
			return ;
		if (shell->value)
		{
			new_node1->line = quotes_in_env(shell->value, shell->key, 1);
			new_node1->next = NULL;
			ft_push_to_env(&(shell->env), new_node1);
		}
		new_node2->line = quotes_in_env(shell->value, shell->key, 0);
		new_node2->next = NULL;
		ft_push_to_env(copy, new_node2);
	}
	else
	{
		new_node1->line = quotes_in_env(shell->value, shell->key, 1);
		new_node1->next = NULL;
		ft_push_to_env(&(shell->env), new_node1);
	}
}

char	*quotes_in_env(char *value, char *key, int flag)
{
	char	*val;
	char	*temp;

	temp = key;
	if (value)
	{
		val = ft_strjoin(temp, "=");
		if (flag)
			temp = ft_strjoin(val, value);
		else
		{
			temp = ft_strjoin(val, "\"");
			free(val);
			val = ft_strjoin(temp, value);
			free(temp);
			temp = ft_strjoin(val, "\"");
		}
		free(val);
		val = ft_strdup(temp);
	}
	else
		val = ft_strdup(temp);
	free (temp);
	return (val);
}

int	handle_export_quotes(char *temp, char *result, size_t *i, size_t *j)
{
	char	quote;

	quote = temp[*i];
	(*i)++;
	while (*i < ft_strlen(temp))
	{
		if (temp[*i] == '\\' && temp[*i + 1] == quote)
		{
			(*i)++;
			result[(*j)++] = temp[*i];
		}
		else if (temp[*i] == quote && !escape(temp, *i))
			break ;
		else
			result[(*j)++] = temp[*i];
		(*i)++;
	}
	if (*i < ft_strlen(temp) && temp[*i] == quote)
		(*i)++;
	else
		return (ft_error_message_quotes(quote), -1);
	result[*j] = '\0';
	return (0);
}
