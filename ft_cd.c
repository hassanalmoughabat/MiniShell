/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:09:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/02/13 13:12:55 by hal-moug         ###   ########.fr       */
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

char	*ft_get_pwd(void)
{
	char	cwd[PATH_MAX];
	
	if (getcwd(cwd, PATH_MAX))
		return cwd;
}

static int	ft_find_old_pwd(char *str)
{
	int	i;

	i = 0;
	while (str[i + 5])
	{
		if (ft_strncmp(str, "OLDPWD", 6) == 0)
		{
				ft_printf("\ncurent->line is %s\n", str);
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

char	*find_dir_in_list(t_token *tk)
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

void update_pwd(t_env *env, char *oldvalue, char *newvalue, char *be_updated)
{
	t_env *curr;

	curr = env;
	if (ft_strncmp(oldvalue, be_updated))
}


void	ft_cd(t_token *tk, t_env *env, char **ft_env)
{
	char *dir;
	char *pwd;
	char *oldpwd;
	
	dir = find_dir_in_list(tk);
	pwd = ft_get_pwd();
	oldpwd = ft_get_old_pwd(env);
	if (!chdir(dir))
		error_print("directory not found", 1);
	update_pwd(env, oldpwd, pwd, "pwd=");
	update_pwd(env, pwd, dir);
}