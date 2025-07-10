/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_cont.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:15:21 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/09 23:27:43 by njoudieh42       ###   ########.fr       */
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
		if (!ft_strcmp(curr->cmd, "cd"))
		{
			if (curr->next)
				return (curr->next->cmd);
			else
				return (NULL);
		}
		curr = curr->next;
	}
	return (NULL);
}

void	ft_cd(t_token *tk, t_env *env, char **ft_env)
{
	char	*dir;
	char	*pwd;
	char	*oldpwd;
	char	*oldpwd_env;
	char	*home;
	int		size;

	home = my_getenv("HOME", ft_env);
	size = ft_list_size(tk);
	dir = find_dir_in_list(tk);
	pwd = ft_get_cd_pwd();
	oldpwd = ft_strdup(pwd);
	oldpwd_env = my_getenv("OLDPWD", ft_env);
	remove_added_quotes(&dir);
	if (size > 2)
	{
		g_minishell.env->exit_status = ft_err_msg((t_error){"cd", ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
		return ;
	}
	if (!oldpwd)
		update_env_value(&env, "PWD", pwd);
	if (dir && !ft_strcmp(dir, "-"))
	{
		if (!oldpwd_env)
		{
			ft_putstr_fd("bash: cd: OLDPWD not set\n", 2);
			g_minishell.env->exit_status = 1;
			return ;
		}
		if (chdir(oldpwd_env) == 0)
		{
			char *cwd = ft_get_cd_pwd();
			ft_putstr_fd(cwd, 1);
			ft_putstr_fd("\n", 1);
			update_env_value(&env, "OLDPWD=", oldpwd);
			update_env_value(&env, "PWD=", cwd);
		}
		else
			g_minishell.env->exit_status = ft_err_msg((t_error){oldpwd_env, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		return ;
	}
	else if ((dir == NULL && size == 1) || (!ft_strcmp(dir, "--")))
		chdir(home);
	else if (chdir(dir) != 0)
		g_minishell.env->exit_status = ft_err_msg((t_error){dir, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
	printf("this is the %s oldpwd and this is %s pwd \n", oldpwd, dir);
	dir = ft_get_cd_pwd();
	update_env_value(&env, "OLDPWD=", oldpwd);
	update_env_value(&env, "PWD=", dir);
}

// void	ft_cd(t_token *tk, t_env *env, char **ft_env)
// {
// 	char	*dir;
// 	char	*pwd;
// 	char	*cd_path;
// 	char	*oldpwd;
// 	char	*home;
// 	char	*temp;
// 	char	cwd[PATH_MAX];

// 	home = my_getenv("HOME", ft_env);
// 	cd_path = my_getenv("CDPATH", ft_env);
// 	dir = find_dir_in_list(tk);
// 	pwd = ft_get_cd_pwd();
// 	remove_ad-ded_quotes(&dir);
// 	if (!pwd)
// 	{
// 		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
// 		return;
// 	}
// 	oldpwd = ft_strdup(get_value_from_env("PWD", env));
// 	if (!ft_strcmp(dir, "~"))
// 	{
// 		if (dir)
// 			free (dir);
// 		dir = ft_strdup(home);
// 	}
// 	if (ft_list_size(tk) > 2)
// 	{
// 		env->exit_status = ft_err_msg((t_error){"cd", ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
// 		return ;
// 	}
// 	if (!oldpwd)
// 		update_env_value(env, "PWD", pwd);
// 	else if (!dir || dir[0] == '\0')
// 	{
// 		if (!home)
// 		{
// 			ft_putstr_fd("bash: cd: HOME not set\n", 2);
// 			if (my_getenv("PWD", ft_env) && !chdir(my_getenv("PWD", ft_env)))
// 				update_env_value(env, "PWD", my_getenv("PWD", ft_env));
// 			env->exit_status = 1;
// 		}
// 		else
// 			chdir(home);
// 	}
// 	if (cd_path && ft_strncmp(dir, "./", 2) && ft_strncmp(dir, "../", 2) && dir[0] != '/')
// 	{
// 		temp = ft_strjoin(cd_path, dir);
// 		if (dir)
// 			free (dir);
// 		dir = ft_strdup(temp);
// 		free(temp);
// 	}
// 	if (chdir(dir) == 0)
// 	{
// 		char *cwd = getcwd(NULL, 0);
// 		if (!cwd)
// 		{
// 			ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
// 		}
// 		else
// 		{
// 			update_env_value(env, "OLDPWD=", oldpwd);
// 			update_env_value(env, "PWD=", cwd);
// 			free(cwd);
// 		}
// 	}
// 	ft_putstr_fd(dir, 1);
// 	ft_putstr_fd("\n", 1);
// }
