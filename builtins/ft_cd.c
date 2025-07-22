/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:09:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/16 22:38:24 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	add_old_pwd_var(char *target, char *new_value, t_env **head)
{
	char	*updated_value;

	updated_value = NULL;
	if (!ft_strcmp(target, "OLDPWD="))
	{
		updated_value = ft_strjoin(target, new_value);
		if (updated_value)
		{
			ft_add_key_to_env(head, updated_value);
			free(updated_value);
		}
	}
	return ;
}

void	update_env_value(t_env **head, char *target, char *new_value)
{
	char	*updated_value;
	size_t	len;
	t_env	*current;

	len = ft_strlen(new_value);
	if (!*head || !target || !new_value)
		return ;
	updated_value = malloc(ft_strlen(target) + len + 1);
	if (!updated_value)
		return ;
	current = *head;
	while (current)
	{
		if (current->line
			&& ft_strncmp(current->line, target, ft_strlen(target)) == 0)
		{
			ft_strcpy(updated_value, target);
			ft_strcat(updated_value, new_value);
			free(current->line);
			current->line = updated_value;
			return ;
		}
		current = current->next;
	}
	add_old_pwd_var(target, new_value, head);
}

char	*ft_get_pwd(void)
{
	char	*cwd;

	cwd = malloc(PATH_MAX);
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, PATH_MAX))
		return (cwd);
	free(cwd);
	return (NULL);
}

int	ft_find_old_pwd(char *str)
{
	int	i;

	i = 0;
	while (str[i + 5])
	{
		if (ft_strncmp(str, "OLDPWD=", 7) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_get_old_pwd(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_find_old_pwd(current->line) == 1)
			return (current->line);
		else
			current = current->next;
	}
	return (0);
}
