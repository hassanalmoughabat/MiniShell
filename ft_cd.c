/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:09:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/04/03 17:35:05 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

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

// static int	ft_find_pwd(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i + 2])
// 	{
// 		if (ft_strncmp(str, "PWD", 3))
// 		{
// 			ft_printf("\ncurent->line is %s\n", str);
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

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

static char	*find_dir_in_list(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if (!(ft_strcmp(curr->cmd, "cd") == 0))
			return (curr->cmd);
		curr = curr->next;
	}
	return (NULL);
}

static char	*ft_get_cd_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	pwd = getcwd(cwd, PATH_MAX);
	return (pwd);
}

void	ft_cd(t_token *tk, t_env *env, char **ft_env)
{
	char	*dir;
	char	*pwd;
	char	*oldpwd;
	char	 *home;
	int		size;

	home = my_getenv("HOME", ft_env);
	size = ft_list_size(tk);
	dir = find_dir_in_list(tk);
	pwd = ft_get_cd_pwd();
	oldpwd = ft_strdup(pwd);
	if (!oldpwd)
		update_env_value(env, "PWD", pwd);
	else if (dir == NULL && size == 1)
		chdir(home);
	else if (chdir(dir) != 0)
		error_print("directory not found\n", 1);
	else
	{
		dir = ft_get_cd_pwd();
		update_env_value(env, "OLDPWD=", oldpwd);
		update_env_value(env, "PWD=", dir);
	}
}
