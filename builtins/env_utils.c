/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:24:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/26 17:00:07 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	free_my_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->line)
			free(tmp->line);
		if (tmp)
			free(tmp);
	}
}

t_env	*create_env_node(char *envp)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->line = ft_strdup(envp);
	new_node->exit_status = ENU_SUCCESS;
	new_node->safe_quotes = false;
	if (!new_node->line)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*initialize_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			free_my_env(head);
			free(envp[i]);
			return (NULL);
		}
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	new_node = create_env_node("_=/usr/bin/env");
	if (!new_node)
	{
		free_my_env(head);
		return (NULL);
	}
	if (!head)
		head = new_node;
	else
		current->next = new_node;
	return (head);
}

char	*get_path(char *cmd, char *envp[])
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	i = 0;
	allpath = ft_split(my_getenv("PATH", envp), ':');
	s_cmd = ft_split(cmd, ' ');
	if (!allpath)
		return (NULL);
	while (allpath[i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(allpath);
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_tab(allpath);
	ft_free_tab(s_cmd);
	return (ft_strdup(cmd));
}
