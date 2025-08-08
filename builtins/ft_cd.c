/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:39:38 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/24 19:53:30 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	handle_empty_dir(t_shell *shell, char *home)
{
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		shell->env->exit_status = 1;
		return ;
	}
	if (ft_strcmp(home, "") && chdir(home))
	{
		shell->env->exit_status = ft_err_msg((t_error){home,
				ERROR_MESG_NO_FILE, ENU_GENEREAL_FAILURE});
		return ;
	}
}

void	set_env_pwd(char **home, char **dir, t_shell *shell)
{
	*home = my_getenv("HOME", shell->ft_env);
	*dir = find_dir_in_list(shell->tk);
	remove_added_quotes(dir);
	remove_added_quotes(home);
}

int	fix_home(char **dir, char *home, int flag, t_shell *shell)
{
	if (flag == 1)
	{
		free(*dir);
		*dir = ft_strdup(home);
		return (1);
	}
	else if (flag == 2)
	{
		if ((!*dir && ft_list_size(shell->tk) == 1)
			|| (*dir && (!ft_strcmp(*dir, "--") || !ft_strcmp(*dir, ""))))
			return (2);
	}
	return (0);
}

void	ft_cd(t_shell *shell)
{
	char	*dir;
	char	*current_pwd;
	char	*old_pwd;
	char	*home;

	if (ft_list_size(shell->tk) > 2)
		return ((void)(error_env(shell, 2)));
	current_pwd = ft_get_cd_pwd();
	if (!current_pwd)
		current_pwd = set_curr_pwd(shell);
	old_pwd = ft_strdup(current_pwd);
	set_env_pwd(&home, &dir, shell);
	if (dir && !ft_strcmp(dir, "-"))
		return ((void)retrieve_dir(shell, old_pwd));
	if (dir && home && (!ft_strcmp(dir, "~") || !ft_strcmp(dir, "~/")))
		fix_home(&dir, home, 1, shell);
	if (fix_home(&dir, home, 2, shell) == 2)
		return ((void)handle_empty_dir(shell, home));
	else if (dir)
	{
		if (change_dir(dir, shell))
			return ;
		update_env_values(shell, old_pwd);
	}
	free(old_pwd);
}
