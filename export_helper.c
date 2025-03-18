/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:29 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/18 16:13:00 by njoudieh42       ###   ########.fr       */
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
		if (current->line
			&& !ft_strncmp(current->line + 11, key, ft_strlen(key))
			&& !ft_strncmp(current->line, "declare -x ", 11))
		{
			value = get_value(current->line);
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

	quote = temp[(*i)];
	(*i)++;
	while (*i < ft_strlen(temp) && temp[*i] != quote)
	{
		if (flag && quote_helper(quote, temp[*i]))
		{
			free(result);
			return (-1);
		}
		result[(*j)++] = temp[(*i)++];
	}
	if (*i < ft_strlen(temp) && temp[*i] == quote)
		(*i)++;
	else
	{
		free(result);
		ft_error_message_quotes(quote);
		return (-1);
	}
	return (0);
}

int	ft_update_env(char *key, char *value, t_env **env)
{
	t_env	*current;
	char	*temp;
	char	*val;

	current = *env;
	while (current)
	{
		if (!ft_strncmp(current->line + 11, key, ft_strlen(key))
			&& !ft_strncmp(current->line, "declare -x ", 11))
		{
			free(current->line);
			temp = ft_strjoin("declare -x ", key);
			if (value)
			{
				val = ft_strjoin(temp, "=\"");
				free (temp);
				temp = val;
				val = ft_strjoin(temp, value);
				free (temp);
				temp = val;
				val = ft_strjoin(temp, "\"");
				free(temp);
				temp = val;
			}
			else
				val = temp;
			current->line = val;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	ft_add_env(char *key, char *value, t_env **env)
{
	t_env	*new_node;
	char	*new_line;
	char	*temp;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_line = ft_strjoin("declare -x ", key);
	if (value)
	{
		temp = ft_strjoin(new_line, "=\"");
		free (new_line);
		new_line = temp;
		temp = ft_strjoin(new_line, value);
		free (new_line);
		new_line = temp;
		temp = ft_strjoin(new_line, "\"");
		free (new_line);
		new_line  = temp;
	}
	ft_push_to_env(env, new_node, new_line);
}
