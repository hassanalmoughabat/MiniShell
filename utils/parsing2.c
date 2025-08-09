/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:42 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:21:52 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	handle_semicolon_error(t_shell *shell, t_token *curr)
{
	if (!ft_strncmp(curr->cmd, ";", 1))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		if (curr->cmd[1] == ';')
			ft_putstr_fd(";;'", 2);
		else
			ft_putstr_fd(";'", 2);
		ft_putstr_fd("\n", 2);
		shell->env->exit_status = 2;
		return (1);
	}
	return (0);
}

void	child_exec(t_shell *shell, char *cmd, char **argv)
{
	char	*path;

	signal(SIGQUIT, SIG_DFL);
	path = get_path(argv[0], shell->ft_env);
	if (!path)
	{
		remove_added_quotes(&cmd);
		shell->env->exit_status = ft_err_msg((t_error){cmd,
				ERROR_MESG_NO_FILE, ENU_CMD_NOT_FOUND});
		exit(127);
	}
	if (contains_symbols(cmd, 1))
		validate_executable(cmd, path, shell);
	if (execve(path, argv, shell->ft_env) == -1)
	{
		shell->env->exit_status = ft_err_msg((t_error){cmd,
				ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
		exit(127);
	}
}

void	parent_wait_and_cleanup(t_shell *shell, pid_t pid, char **argv)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		shell->env->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		shell->env->exit_status = WEXITSTATUS(status);
	else
		shell->env->exit_status = 0;
	free_shell_args(argv);
}

int	handle_empty_cmd(t_shell *shell, char *cmd)
{
	if (!ft_strcmp(cmd, "\"\"") || !ft_strcmp(cmd, "''"))
	{
		remove_added_quotes(&cmd);
		shell->env->exit_status = ft_err_msg((t_error){cmd,
				ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
		return (1);
	}
	return (0);
}

int	ft_is_builtin(char *cmd)
{
	char	*temp;

	temp = ft_strdup(cmd);
	remove_added_quotes(&temp);
	if ((ft_strcmp(temp, "cd") == 0))
		return (1);
	else if (ft_strcmp(temp, "echo") == 0)
		return (1);
	else if (ft_strcmp(temp, "pwd") == 0)
		return (1);
	else if (ft_strcmp(temp, "exit") == 0)
		return (1);
	else if (ft_strcmp(temp, "unset") == 0)
		return (1);
	else if (ft_strcmp(temp, "export") == 0)
		return (1);
	else if (ft_strcmp(temp, "env") == 0)
		return (1);
	return (0);
}
