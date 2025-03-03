/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:09:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/03/03 11:06:03 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"


static int	ft_find_pwd(char *str)
{
	int	i;

	i = 0;
	while (str[i + 2])
	{
		if (ft_strncmp(str, "PWD", 3))
		{
				ft_printf("\ncurent->line is %s\n", str);
				return (1);
		}
		i++;
	} 
	return (0);
}

static char	*ft_get_pwd(void)
{
	char	cwd[PATH_MAX];
	
	if (getcwd(cwd, PATH_MAX))
		return cwd;
}

int	ft_find_old_pwd(char *str)
{
	int	i;

	i = 0;
	while (str[i + 5])
	{
		if (ft_strncmp(str, "OLDPWD=", 7) == 0)
		{
				return (1);
		}
		i++;
	} 
	return (0);
}

char	*ft_get_old_pwd(t_env *env)
{
	t_env *current;

	current = env;
	while (current)
	{
		if (ft_find_old_pwd(current->line) == 1)
		{	
			return current->line;
		}
		else 
			current = current->next;
	}
	return (0);
}

static char	*find_dir_in_list(t_token *tk)
{
	t_token *curr;
	
	curr = tk;
	while (curr)
	{
		if (!(ft_strcmp(curr->cmd, "cd") == 0))
			return curr->cmd;
		curr = curr->next;
	}
	return (NULL);
}

void	update_env_value(t_env *head, const char *target, const char *new_value)
{
	while(head)
	{
		if (ft_strncmp(target, new_value, ft_strlen(target)) == 0)
		{
			free(head->line);
			head->line = ft_strdup(new_value);
			if (!head->line)
			{
				error_print("failed to allocate memory", 1);
				exit(EXIT_FAILURE);
			}
			return ;
		}
		head = head->next;
	}
}


void	ft_cd(t_token *tk, t_env *env, char **ft_env)
{
	char *dir;
	char *pwd;
	char *oldpwd;
	
	dir = find_dir_in_list(tk);
	pwd = ft_get_pwd();
	oldpwd = ft_get_old_pwd(env);
	if (!oldpwd)
		update_env_value(tk, "PWD", pwd);
	if (chdir(dir) != 0)
	{
			error_print("directory not found\n", 1);
	}
	else
	{
		update_env_value(tk, "OLDPWD=", pwd);
		update_env_value(tk, "PWD=", dir);
	}
}