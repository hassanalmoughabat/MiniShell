/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:29 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 03:04:11 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

char	*get_value_from_env(char *key, t_env *env)
{
	char	*value;
	t_env	*current;

	current = env;
	value = NULL;
	while (current)
	{
		if (current->line && extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
		{
			value = get_value(current->line,env);
			remove_added_quotes(&value, 0);
			return (value);
		}
		current = current->next;
	}
	return (value);
}

int	quote_helper(char c, char val)
{
	if (val != c && ft_check_quotes(val))
		return (1);
	return (0);
}
int	handle_export_quotes(char *temp, char *result, size_t *i, size_t *j, int flag)
{
	char	quote;

	quote = temp[*i];
	(*i)++;
	while (*i < ft_strlen(temp))
	{
		if (temp[*i] == quote && !escape(temp, *i))
			break;
		if (flag && quote_helper(quote, temp[*i]))
			return (free(result),-1);
		result[(*j)++] = temp[*i];
		(*i)++;
	}
	if (*i < ft_strlen(temp) && temp[*i] == quote)
		(*i)++;
	else
	{
		free(result);
		ft_error_message_quotes(quote);
		return (-1);
	}
	result[*j] = '\0';
	return (0);
}

size_t	extract_key_env(char *line)
{
	size_t	len;

	len = 0;
	while (line[len] && line[len] != '=')
		len ++;
	return  (len);
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
		val = temp;
	}
	else
		val = temp;
	return (val);
}

int	ft_update_env(char *key, char *value, t_env **env, t_env **copy)
{
	t_env	*current;
	t_env	*curr1;
	
	current = *env;
	curr1 = *copy;
	while (current && curr1)
	{
		if (extract_key_env(current->line) == ft_strlen(key) && !ft_strncmp(current->line, key, ft_strlen(key)))
		{
			free(current->line);
			free(curr1->line);
			current->line = quotes_in_env(value, key, 1);
			curr1->line = quotes_in_env(value, key, 0);
			return (1);
		}
		current = current->next;
		curr1 = curr1->next;
	}
	
	return (0);
}

void	ft_add_env(char *key, char *value, t_env **env, t_env **copy)
{
	t_env	*new_node1;
	t_env	*new_node2;


	new_node1 = (t_env *)malloc(sizeof(t_env));
	new_node2 = (t_env *)malloc(sizeof(t_env));
	if (!new_node1 || !new_node2)
		return ;
	new_node1->line = quotes_in_env(value, key, 1);
	new_node1->next = NULL;
	new_node2->line = quotes_in_env(value, key, 0);
	new_node2->next = NULL;
	ft_push_to_env(env, new_node1);
	ft_push_to_env(copy, new_node2);
}
