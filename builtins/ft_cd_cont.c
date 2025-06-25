/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_cont.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:15:21 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/25 22:53:57 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

static char	*ft_get_cd_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	pwd = getcwd(cwd, PATH_MAX);
	return (pwd);
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

void	ft_cd(t_token *tk, t_env *env, char **ft_env)
{
	char	*dir;
	char	*pwd;
	char	*oldpwd;
	char	*home;
	int		size;

	// display_list(tk);
	home = my_getenv("HOME", ft_env);
	size = ft_list_size(tk);
	dir = find_dir_in_list(tk);
	pwd = ft_get_cd_pwd();
	oldpwd = ft_strdup(pwd);
	if (size > 2)
	{
		g_minishell.env->exit_status = ft_err_msg((t_error){"cd", ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
		return ;
	}
	if (!oldpwd)
		update_env_value(env, "PWD", pwd);
	else if (dir == NULL && size == 1)
		chdir(home);
	else if (chdir(dir) != 0)
		g_minishell.env->exit_status = ft_err_msg((t_error){dir, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
	else
	{
		dir = ft_get_cd_pwd();
		update_env_value(env, "OLDPWD=", oldpwd);
		update_env_value(env, "PWD=", dir);
	}
}
