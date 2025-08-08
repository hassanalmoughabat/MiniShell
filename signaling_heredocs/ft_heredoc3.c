/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:04:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:06:47 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*create_new_line(char *line, char *pos,
	char *var_name, char *new_val)
{
	size_t	before_len;
	size_t	var_name_len;
	size_t	new_len;
	char	*new_line;

	before_len = pos - line;
	var_name_len = ft_strlen(var_name);
	new_len = before_len + ft_strlen(new_val) + ft_strlen(pos + var_name_len);
	new_line = malloc(new_len + 1);
	if (!new_line)
		return (NULL);
	strncpy(new_line, line, before_len);
	ft_strcpy(new_line + before_len, new_val);
	ft_strcpy(new_line + before_len + ft_strlen(new_val), pos + var_name_len);
	return (new_line);
}

char	*replace_variable(char *line, char *var_name, char *new_val)
{
	char	*pos;

	pos = strstr(line, var_name);
	if (!pos)
		return (ft_strdup(line));
	return (create_new_line(line, pos, var_name, new_val));
}

char	*extract_variable(const char *line)
{
	char	prefix;
	char	*var_start;
	char	*var_end;
	size_t	var_len;
	char	*variable;

	prefix = '$';
	var_start = strchr(line, prefix);
	if (!var_start)
		return (NULL);
	var_end = var_start + 1;
	while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
		var_end++;
	var_len = var_end - var_start;
	variable = malloc(var_len + 1);
	if (!variable)
		return (NULL);
	strncpy(variable, var_start, var_len);
	variable[var_len] = '\0';
	return (variable);
}

void	find_quote_bounds(char *line, size_t *start, size_t *end)
{
	*start = 0;
	*end = strlen(line);
	while (line[*start] == '\'' && *start < *end)
		(*start)++;
	while (*end > *start && line[*end - 1] == '\'')
		(*end)--;
}

char	*extract_variable_quote(char *line)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*result;

	if (!line)
		return (NULL);
	find_quote_bounds(line, &start, &end);
	len = end - start;
	if (len == 0)
		return (NULL);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	strncpy(result, line + start, len);
	result[len] = '\0';
	return (result);
}
