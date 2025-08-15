/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/14 22:33:50 by njoudieh42       ###   ########.fr       */
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
		_exit(1);
	}
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	execve("/bin/cat", (char *[]){"cat", NULL}, ft_env);
	_exit(127);
}

pid_t	create_output_child(int *pipefd, t_token *redirect_token,
			char **ft_env)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
		return (-1);
	if (pid2 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		setup_output_redirect_child(pipefd, redirect_token, ft_env);
	}
	return (pid2);
}
