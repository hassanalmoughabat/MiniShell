/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc_util2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 02:43:10 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/16 10:16:27 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	handle_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->env->exit_status = 128 + WTERMSIG(status);
	if (WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

static int	handle_child_creation_failure(t_heredoc_pipe_params *params)
{
	close(params->pipefd[0]);
	close(params->pipefd[1]);
	free(params->delimiter);
	kill(params->pid1, SIGTERM);
	waitpid(params->pid1, NULL, 0);
	return (0);
}

static int	handle_interrupted_heredoc(pid_t pid2, t_shell *shell)
{
	int	status2;

	kill(pid2, SIGKILL);
	waitpid(pid2, &status2, 0);
	shell->env->exit_status = 130;
	ft_restore_main_signals();
	return (1);
}

int	handle_heredoc_pipe_redirect_part2(t_heredoc_pipe_params *params,
			t_shell *shell)
{
	pid_t	pid2;
	int		status1;
	int		status2;

	pid2 = create_output_child(params->pipefd, params->redirect_token,
			params->ft_env);
	if (pid2 == -1)
		return (handle_child_creation_failure(params));
	close(params->pipefd[0]);
	close(params->pipefd[1]);
	free(params->delimiter);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(params->pid1, &status1, 0);
	if (WIFSIGNALED(status1) && WTERMSIG(status1) == SIGINT)
		return (handle_interrupted_heredoc(pid2, shell));
	if (WIFEXITED(status1) && WEXITSTATUS(status1) == 130)
		return (handle_interrupted_heredoc(pid2, shell));
	waitpid(pid2, &status2, 0);
	handle_exit_status(shell, status2);
	ft_restore_main_signals();
	return (1);
}
