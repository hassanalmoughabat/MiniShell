/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:15:21 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/24 19:17:12 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*ft_get_cd_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	pwd = getcwd(cwd, PATH_MAX);
	return (pwd);
}

char	*find_dir_in_list(t_token *tk)
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

char	*set_curr_pwd(t_shell *shell)
{
	char	*old_pwd_env;

	old_pwd_env = get_value_from_env("PWD", shell->env);
	if (old_pwd_env && old_pwd_env[0])
		return (ft_strdup(old_pwd_env));
	else
		return (ft_strdup(""));
}

void	error_env(t_shell *shell, int flag)
{
	if (flag == 1)
	{
		ft_putstr_fd("bash: cd: OLDPWD not set\n", 2);
		shell->env->exit_status = 1;
	}
	else if (flag == 2)
	{
		shell->env->exit_status = ft_err_msg
			((t_error){"cd", ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
	}
	return ;
}

void	retrieve_dir(t_shell *shell, char *old_pwd)
{
	char	*oldpwd_env;
	char	*new_pwd;

	oldpwd_env = get_value_from_env("OLDPWD", shell->env);
	if (!oldpwd_env || !oldpwd_env[0])
		return ((void)error_env(shell, 1));
	if (chdir(oldpwd_env) == 0)
	{
		new_pwd = ft_get_cd_pwd();
		if (new_pwd)
		{
			update_env_value(&(shell->env), "OLDPWD=", old_pwd);
			update_env_value(&(shell->env), "PWD=", new_pwd);
		}
		else
		{
			update_env_value(&(shell->env), "OLDPWD=", old_pwd);
			shell->env->exit_status = ft_err_msg((t_error){old_pwd,
					ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		}
	}
	else
		shell->env->exit_status = ft_err_msg((t_error){oldpwd_env,
				ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
	return ;
}
