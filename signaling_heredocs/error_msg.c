/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:46:54 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 00:47:26 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	handle_directory_error(t_token *cmd, t_shell *shell)
{
	shell->env->exit_status = 126;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->cmd, 2);
	ft_putstr_fd(" Is a directory\n", 2);
	exit(126);
}

void	handle_path_error(t_token *cmd, t_shell *shell)
{
	remove_added_quotes(&cmd->cmd);
	shell->env->exit_status = ft_err_msg((t_error){cmd->cmd,
			ERROR_MESG_NO_FILE, ENU_CMD_NOT_FOUND});
	exit(127);
}

void	handle_permission_error(t_token *cmd, t_shell *shell)
{
	shell->env->exit_status = 126;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->cmd, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	exit(126);
}

void	handle_no_such_file(t_token *cmd, t_shell *shell)
{
	shell->env->exit_status = 127;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->cmd, 2);
	ft_putstr_fd(" No such file or directory\n", 2);
	exit(127);
}
