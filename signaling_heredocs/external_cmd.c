/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/27 00:47:37 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	check_executable(char *path, t_token *cmd, t_shell *shell)
{
	struct stat	sb;

	if (!stat(path, &sb))
	{
		if (S_ISDIR(sb.st_mode))
			handle_directory_error(cmd, shell);
		else if (access(path, X_OK) != 0)
			handle_permission_error(cmd, shell);
	}
	else
		handle_no_such_file(cmd, shell);
}

void	execute_external_cmd(t_token *cmd_tokens, char **ft_env, t_shell *shell)
{
	char		**args;
	char		*path;

	args = build_args_array(cmd_tokens, &shell->gc);
	if (!args)
		exit(EXIT_FAILURE);
	path = get_path(args[0], ft_env);
	signal(SIGQUIT, SIG_DFL);
	path = get_path(args[0], shell->ft_env);
	if (!path)
		handle_path_error(cmd_tokens, shell);
	if (symbols(cmd_tokens->cmd))
		check_executable(path, cmd_tokens, shell);
	if (execve(path, args, shell->ft_env) == -1)
	{
		shell->env->exit_status = ft_err_msg((t_error){(cmd_tokens->cmd),
				ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
		exit(127);
	}
}
