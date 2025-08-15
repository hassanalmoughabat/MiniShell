/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:07:23 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:08:10 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*find_after_delim(t_token *curr, char *delimiter)
{
	while (curr)
	{
		if (ft_strcmp(curr->cmd, delimiter) == 0 && curr->next)
			return (curr->next->cmd);
		curr = curr->next;
	}
	return (NULL);
}

char	*find_command_around_heredoc(t_token *tk, char *delimiter)
{
	t_token	*curr;
	t_token	*command_before;
	char	*cmd;

	curr = tk;
	command_before = NULL;
	while (curr)
	{
		if (curr->next && ft_strcmp(curr->next->cmd, "<<") == 0)
			command_before = curr;
		if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
		{
			cmd = find_after_delim(curr->next, delimiter);
			if (cmd)
				return (cmd);
			if (command_before)
				return (command_before->cmd);
			return (NULL);
		}
		curr = curr->next;
	}
	return (NULL);
}

char	*extract_var_name(char *start, char *end)
{
	size_t	len;
	char	*name;

	len = end - start;
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	strncpy(name, start, len);
	name[len] = '\0';
	return (name);
}

char	*handle_single_variable(char *result, char *start, t_env *env, t_gc *gc)
{
	char	*end;
	char	*name;
	char	*value;
	char	*temp;

	end = start + 1;
	while (*end && (isalnum(*end) || *end == '_'))
		end++;
	name = extract_var_name(start, end);
	if (!name)
		return (NULL);
	value = cut_from_op('$', start, env, gc);
	if (!value)
		value = ft_strdup_gc(gc, "");
	temp = replace_variable(result, name, value);
	return (temp);
}

char	*process_all_variables(char *line, t_env *env, t_gc *gc)
{
	char	*result;
	char	*start;

	result = ft_strdup_gc(gc, line);
	if (!result)
		return (NULL);
	start = ft_strchr(result, '$');
	while (start)
	{
		result = handle_single_variable(result, start, env, gc);
		if (!result)
			return (NULL);
	}
	return (result);
}
