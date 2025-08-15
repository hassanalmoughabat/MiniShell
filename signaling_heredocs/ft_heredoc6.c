/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:08:24 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 19:14:49 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	process_expansion(char **src, t_expand_data *data, t_env *env, t_gc *gc)
{
	char	*var_name;

	(*src)++;
	var_name = extract_var(src);
	if (!var_name)
		return (0);
	if (!expand_variable(data, var_name, env, gc))
		return (0);
	return (1);
}

char	*expand_variables(char *line, t_env *env, int quote, t_gc *gc)
{
	t_expand_data	data;
	char			*src;

	if (quote == 1 || !line || !ft_strchr(line, '$'))
		return (ft_strdup_gc(gc, line));
	if (!init_expand_data(&data))
		return (NULL);
	src = line;
	while (*src)
	{
		if (*src == '$' && (ft_isalpha(src[1]) || src[1] == '_'))
		{
			if (!process_expansion(&src, &data, env, gc))
				return (NULL);
		}
		else
		{
			if (!copy_char(&data, *src))
				return (NULL);
			src++;
		}
	}
	*data.dst = '\0';
	return (data.result);
}
