/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:50:02 by hal-moug          #+#    #+#             */
/*   Updated: 2025/05/17 23:17:07 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*path_extract(char *str, int count, t_gc *gc)
{
	int				i;
	int				word_len;
	char			*path;
	int				j;

	i = count;
	while (str[i])
		i++;
	word_len = i - count;
	path = ft_malloc(gc, sizeof(char) * (word_len + 1));
	if (!path)
		return (0);
	j = 0;
	while (str[count])
	{
		path[j] = str[count];
		j++;
		count++;
	}
	path[j] = '\0';
	return (path);
}

char	*get_my_path(t_env *env, t_gc *gc)
{
	t_env	*current;
	char	*path;

	if (!env)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strncmp(current->line, "PATH=", 5) == 0)
		{
			path = path_extract(current->line, 5, gc);
			env->exit_status = ENU_SUCCESS;
			return (path);
		}
		current = current->next;
	}
	return (NULL);
}

char	**transform(t_env *env, t_gc *gc)
{
	char	**ftenvp;
	int		count;
	int		i;
	t_env	*current;

	current = env;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	ftenvp = (char **)ft_malloc(gc, sizeof(char *) * (count + 1));
	if (!ftenvp)
		return (NULL);
	current = env;
	i = 0;
	while (current && i < count)
	{
		ftenvp[i] = ft_strdup_gc(gc, current->line);
		i++;
		current = current->next;
	}
	ftenvp[i] = NULL;
	return (ftenvp);
}

char	*my_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j, NULL);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}
