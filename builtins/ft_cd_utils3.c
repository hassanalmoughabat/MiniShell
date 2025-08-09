/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:10:38 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/08 00:47:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	update_env_values(t_shell *shell, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = ft_get_cd_pwd();
	if (new_pwd)
	{
		update_env_value(&(shell->env), "OLDPWD=", old_pwd);
		update_env_value(&(shell->env), "PWD=", new_pwd);
	}
	else
	{
		update_env_value(&(shell->env), "OLDPWD=", old_pwd);
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		shell->env->exit_status = ENU_SUCCESS;
	}
}

int	change_dir(char *dir, t_shell *shell)
{
	struct stat	st;

	if (stat(dir, &st) != 0)
	{
		shell->env->exit_status = ft_err_msg(
				(t_error){dir, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		return (1);
	}
	if (!S_ISDIR(st.st_mode))
	{
		shell->env->exit_status = ft_err_msg(
				(t_error){dir, ERROR_MESG_NOT_DIR, ENU_GENEREAL_FAILURE});
		return (1);
	}
	if (chdir(dir) != 0)
	{
		shell->env->exit_status = ft_err_msg(
				(t_error){dir, ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		return (1);
	}
	return (0);
}
