/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/27 00:47:37 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "ft_heredoc.h"

char	*ft_init_expansion_buffer(char *line, char **src, char **dst)
{
	char	*result;

	result = malloc(1024);
	if (!result)
		return (NULL);
	*src = line;
	*dst = result;
	return (result);
}

char	*get_env_value_safe(char *var_name, t_env *env)
{
	t_env	*current;
	char	*equals_pos;
	int		name_len;

	name_len = ft_strlen(var_name);
	current = env;
	while (current)
	{
		if (current->line && ft_strncmp(current->line, var_name, name_len) == 0)
		{
			equals_pos = ft_strchr(current->line, '=');
			if (equals_pos && equals_pos == current->line + name_len)
				return (equals_pos + 1);
		}
		current = current->next;
	}
	return ("");
}

static void	expand_variable(char *src, int *i, char **dst, t_env *env)
{
	char	var_name[256];
	char	*value;
	int		j;

	(*i)++;
	j = 0;
	while (src[*i] && (ft_isalnum(src[*i]) || src[*i] == '_') && j < 255)
		var_name[j++] = src[(*i)++];
	var_name[j] = '\0';
	value = get_env_value_safe(var_name, env);
	if (value && *value)
	{
		ft_strcpy(*dst, value);
		*dst += ft_strlen(value);
	}
}

char	*expand_variables_safe(char *line, t_env *env)
{
	char	*result;
	char	*dst;
	int		i;

	result = malloc(ft_strlen(line) * 4 + 1);
	if (!result)
		return (ft_strdup(line));
	dst = result;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]
			&& (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
			expand_variable(line, &i, &dst, env);
		else
			*dst++ = line[i++];
	}
	*dst = '\0';
	return (result);
}

int	ft_process_heredoc_line(char *line, t_heredoc_data *data)
{
	char	*expanded_line;

	if (!line || ft_strcmp(line, data->delimiter) == 0)
	{
		if (line)
			free(line);
		return (1);
	}
	if (data->quote == 1 || !line || !ft_strchr(line, '$'))
		expanded_line = ft_strdup(line);
	else
		expanded_line = expand_variables_safe(line, data->shell->env);
	if (ft_check_heredoc_size(*(data->total_written), expanded_line,
			data->pipefd) == -1)
	{
		free(line);
		free(expanded_line);
		return (-1);
	}
	ft_write_heredoc_line(data->pipefd[1], expanded_line, data->total_written);
	free(line);
	free(expanded_line);
	return (0);
}
