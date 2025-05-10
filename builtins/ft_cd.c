/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:09:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/05/06 23:02:42 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	update_env_value(t_env *head, const char *target, const char *new_value)
{
	char	*updated_value;
	size_t	len;

	len = ft_strlen(new_value);
	updated_value = NULL;
	if (!head || !target || !new_value)
		return ;
	while (head)
	{
		if (head->line
			&& ft_strncmp(head->line, target, ft_strlen(target)) == 0)
		{
			updated_value = malloc(ft_strlen(target) + len + 1);
			if (!updated_value)
				return ;
			strcpy(updated_value, target);
			strcat(updated_value, new_value);
			free(head->line);
			head->line = updated_value;
			return ;
		}
		head = head->next;
	}
	return ;
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
