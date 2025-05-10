/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:50:02 by hal-moug          #+#    #+#             */
/*   Updated: 2025/05/05 12:10:36 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*path_extract(char *str, int count)
{
	int				i;
	int				word_len;
	char			*path;
	int				j;

	i = count;
	while (str[i])
		i++;
	word_len = i - count;
	path = malloc(sizeof(char) * (word_len + 1));
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

char	*get_my_path(t_env *env)
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
			path = path_extract(current->line, 5);
			env->exit_status = ENU_SUCCESS;
			return (path);
		}
		current = current->next;
	}
	return (NULL);
}

char	**transform(t_env *env)
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
	ftenvp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!ftenvp)
		return (NULL);
	current = env;
	i = 0;
	while (current && i < count)
	{
		ftenvp[i] = ft_strdup(current->line);
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
		sub = ft_substr(env[i], 0, j);
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
