/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_cont.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:15:21 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 15:13:13 by njoudieh42       ###   ########.fr       */
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

void	ft_cd(t_shell *shell)
{
	char	*dir;
	char	*current_pwd;
	char	*oldpwd_env;
	char	*home;
	int		size;

	size = ft_list_size(shell->tk);
	if (size > 2)
	{
		shell->env->exit_status = ft_err_msg((t_error){"cd", ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
		return ;
	}
	current_pwd = ft_get_cd_pwd();
	if (!current_pwd)
	{
		char *old_pwd_env = get_value_from_env("PWD", shell->env);
		if (old_pwd_env && old_pwd_env[0])
			current_pwd = ft_strdup(old_pwd_env);
		else
			current_pwd = ft_strdup("");
	}
	char *old_pwd = ft_strdup(current_pwd);
	home = my_getenv("HOME", shell->ft_env);
	dir = find_dir_in_list(shell->tk);
	remove_added_quotes(&dir);
	remove_added_quotes(&home);
	if (dir && !ft_strcmp(dir, "-"))
	{
		oldpwd_env = get_value_from_env("OLDPWD", shell->env);
		if (!oldpwd_env || !oldpwd_env[0])
		{
			ft_putstr_fd("bash: cd: OLDPWD not set\n", 2);
			shell->env->exit_status = 1;
			return ;
		}
		if (chdir(oldpwd_env) == 0)
		{
			char *new_pwd = ft_get_cd_pwd();
			if (new_pwd)
			{
				update_env_value(&(shell->env), "OLDPWD=", old_pwd);
				update_env_value(&(shell->env), "PWD=", new_pwd);
			}
			else
			{
				update_env_value(&(shell->env), "OLDPWD=", old_pwd);
				shell->env->exit_status = ft_err_msg((t_error){old_pwd, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
			}
		}
		else
			shell->env->exit_status = ft_err_msg((t_error){oldpwd_env, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		return ;
	}
	if (dir && home && (!ft_strcmp(dir, "~") || !ft_strcmp(dir, "~/")))
	{
		free(dir);
		dir = ft_strdup(home);
	}
	if ((!dir && size == 1) || (dir && (!ft_strcmp(dir, "--") || !ft_strcmp(dir, ""))))
	{
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			shell->env->exit_status = 1;
			return ;
		}
		if (ft_strcmp(home, "") && chdir(home))
		{
			shell->env->exit_status = ft_err_msg((t_error){home, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
			return ;
		}
	}
	else if (dir && chdir(dir) != 0)
	{
		shell->env->exit_status = ft_err_msg((t_error){dir, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		return ;
	}
	char *new_pwd = ft_get_cd_pwd();
	if (new_pwd)
	{
		update_env_value(&(shell->env), "OLDPWD=", old_pwd);
		update_env_value(&(shell->env), "PWD=", new_pwd);
	}
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
		shell->env->exit_status = ENU_GENEREAL_FAILURE;
	}
	free(old_pwd);
}

