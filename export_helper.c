/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:29 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/01 21:10:39 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

char	*get_value_from_env(char *key ,t_env *env)
{
	char	*value;
	t_env	*current;
	int		i;
	int		len;

	i = 0;
	current = env;
	value = NULL;
	while (current)
	{
		if (current->line && extract_key_env(current->line) == ft_strlen(key)
			&& !ft_strncmp(current->line, key, ft_strlen(key)))
		{
			while (current->line && current->line[i] != '=')
				i ++;
			len = ft_strlen(current->line) - i -1;
			value = ft_substr(current->line, i + 1, len);
			if (remove_added_quotes(&value, 0) == -1)
			// 	return(free(value),NULL);
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
		if (temp[*i] == '\\' && temp[*i + 1] == quote)
		{
			(*i)++;
			result[(*j)++] = temp[*i];
		}
		else if (temp[*i] == quote && !escape(temp, *i))
			break;
		else
			result[(*j)++] = temp[*i];
		(*i)++;
	}
	if (*i < ft_strlen(temp) && temp[*i] == quote)
		(*i)++;
	else
		return(ft_error_message_quotes(quote), -1);
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
char	*get_var(char *input)
{
	size_t	i;
	char	*key;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i] && input[i] != '=')
		i ++;
	key = ft_substr(input, 0, i);
	if (!check_valid_key(key))
		return (NULL);
	return (key);
}
int	ft_update_env(char *key, char *value, t_env **env, t_env **copy)
{
	t_env	*current;
	t_env	*curr1;
	char	*extract_key;
	
	current = *env;
	curr1 = *copy;
	if (check_if_var_exist(copy, key) && check_if_var_exist(env, key))
	{
		while (curr1 && current)
		{
			extract_key = get_var(curr1->line);
			if (!ft_strcmp(extract_key,key) && ft_strlen(key) == ft_strlen(extract_key))
			{
				free(current->line);
				free(curr1->line);
				current->line = quotes_in_env(value, key, 1);
				curr1->line = quotes_in_env(value, key, 0);
				free(extract_key);
				return (1);
			}
			free(extract_key);
			current = current->next;
			curr1 = curr1->next;
		}
	}
	else if (check_if_var_exist(copy, key) && !check_if_var_exist(env, key))
	{
		while (curr1)
		{
			extract_key = get_var(curr1->line);
			if (!ft_strcmp(extract_key,key) && ft_strlen(key) == ft_strlen(extract_key))
			{
				free(curr1->line);
				curr1->line = quotes_in_env(value, key, 0);
				ft_add_env(key, value, env, copy, 0);
				return (1);
			}
			free(extract_key);
			curr1 = curr1->next;	
		}
	}
	return (0);
}

void	ft_add_env(char *key, char *value, t_env **env, t_env **copy,int flag)
{
	t_env	*new_node1;
	t_env	*new_node2;

	new_node1 = (t_env *)malloc(sizeof(t_env));
	if (flag)
	{
		new_node2 = (t_env *)malloc(sizeof(t_env));
		if (!new_node1 || !new_node2)
			return ;
		if (value)
		{
			new_node1->line = quotes_in_env(value, key, 1);
			new_node1->next = NULL;
			ft_push_to_env(env, new_node1);
		}
		new_node2->line = quotes_in_env(value, key, 0);
		new_node2->next = NULL;
		ft_push_to_env(copy, new_node2);
	}
	else
	{
		new_node1->line = quotes_in_env(value, key, 1);
		new_node1->next = NULL;
		ft_push_to_env(env, new_node1);
	}
}
