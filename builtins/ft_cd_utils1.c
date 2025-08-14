/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:09:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/22 22:46:17 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	add_old_pwd_var(char *target, char *new_value, t_env **head, t_gc *gc)
{
	char	*updated_value;

	updated_value = NULL;
	if (!ft_strcmp(target, "OLDPWD="))
	{
		updated_value = ft_strjoin(target, new_value, gc);
		if (updated_value)
		{
			ft_add_key_to_env(head, updated_value, gc);
			if (!gc)
				free(updated_value);
		}
	}
	return ;
}

void	update_env_value(t_env **head, char *target, char *new_value, t_gc *gc)
{
	char	*updated_value;
	size_t	len;
	t_env	*current;

	len = ft_strlen(new_value);
	if (!*head || !target || !new_value)
		return ;
	updated_value = ft_malloc(gc, ft_strlen(target) + len + 1);
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
			current->line = updated_value;
			return ;
		}
		current = current->next;
	}
	add_old_pwd_var(target, new_value, head, gc);
}

char	*ft_get_pwd(t_gc *gc)
{
	char	*cwd;

	cwd = ft_malloc(gc, PATH_MAX);
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, PATH_MAX))
		return (cwd);
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
