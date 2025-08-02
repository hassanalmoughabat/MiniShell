/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 03:00:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 03:00:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

static void	ft_execute_with_cmd(t_shell *shell, char *delimiter,
				char *cmd, int quote)
{
	char	*args[2];
	char	*path;
	int		read_fd;

	args[0] = cmd;
	args[1] = NULL;
	path = get_path(cmd, shell->ft_env);
	read_fd = handle_dless(delimiter, shell, quote);
	if (read_fd < 0)
	{
		free(delimiter);
		exit(1);
	}
	dup2(read_fd, STDIN_FILENO);
	close(read_fd);
	execve(path, args, shell->ft_env);
	exit(EXIT_FAILURE);
}

static void	ft_execute_without_cmd(char *delimiter, t_shell *shell, int quote)
{
	int	read_fd;

	read_fd = handle_dless(delimiter, shell, quote);
	if (read_fd < 0)
	{
		free(delimiter);
		exit(1);
	}
	close(read_fd);
	exit(EXIT_SUCCESS);
}

void	ft_handle_heredoc_child(t_shell *shell, char *delimiter,
		char *cmd, int quote)
{
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (cmd)
		ft_execute_with_cmd(shell, delimiter, cmd, quote);
	else
		ft_execute_without_cmd(delimiter, shell, quote);
}

void	ft_handle_heredoc_parent(t_shell *shell, pid_t pid, int status)
{
	waitpid(pid, &status, 0);
	g_signal.signint_child = false;
	if (WIFSIGNALED(status))
	{
		shell->env->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			g_signal.heredoc_sigint = true;
	}
	else
		shell->env->exit_status = WEXITSTATUS(status);
}
