/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_great_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/28 16:37:38 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	segment_has_heredoc(t_token *start, t_token *end)
{
	t_token	*curr;

	curr = start;
	while (curr && curr != end)
	{
		if (curr->type == T_DLESS)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	get_heredoc_fd_for_segment(t_token *start, t_token *end,
										t_heredoc_info *heredocs, int hd_count)
{
	int		heredoc_count;
	t_token	*curr;

	heredoc_count = 0;
	curr = start;
	while (curr && curr != end)
	{
		if (curr->type == T_DLESS)
		{
			if (heredoc_count < hd_count)
				return (heredocs[heredoc_count].fd);
			heredoc_count++;
		}
		curr = curr->next;
	}
	return (-1);
}

void	setup_output_redirect_child(int *pipefd, t_token *redirect_token,
			char **ft_env)
{
	int	out_fd;

	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	if (redirect_token->type == T_DGREAT)
		out_fd = open(redirect_token->next->cmd, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	else
		out_fd = open(redirect_token->next->cmd, O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	if (out_fd == -1)
	{
		perror("minishell");
		exit(1);
	}
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	execve("/bin/cat", (char *[]){"cat", NULL}, ft_env);
	exit(127);
}

pid_t	create_output_child(int *pipefd, t_token *redirect_token,
			char **ft_env)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
		return (-1);
	if (pid2 == 0)
		setup_output_redirect_child(pipefd, redirect_token, ft_env);
	return (pid2);
}

int	handle_heredoc_pipe_redirect_part2(t_heredoc_pipe_params *params)
{
	pid_t	pid2;
	int		status;

	pid2 = create_output_child(params->pipefd, params->redirect_token,
			params->ft_env);
	if (pid2 == -1)
	{
		close(params->pipefd[0]);
		close(params->pipefd[1]);
		free(params->delimiter);
		kill(params->pid1, SIGTERM);
		waitpid(params->pid1, NULL, 0);
		return (0);
	}
	close(params->pipefd[0]);
	close(params->pipefd[1]);
	free(params->delimiter);
	waitpid(params->pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		params->env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		params->env->exit_status = 128 + WTERMSIG(status);
	return (1);
}
