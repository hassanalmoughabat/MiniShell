/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/12 20:13:49 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	setup_heredoc_pipe(t_token *lst, t_heredoc_setup_params *setup_params)
{
	t_token					*heredoc_token;
	t_token					*pipe_token;
	t_token					*redirect_token;
	char					*delimiter;

	find_tokens(lst, &heredoc_token, &pipe_token, &redirect_token);
	if (!heredoc_token || !pipe_token || !redirect_token
		|| !redirect_token->next)
		return (0);
	delimiter = get_delimeter(lst, setup_params->gc);
	if (!delimiter)
		return (0);
	setup_params->pipe_params->delimiter = delimiter;
	setup_params->pipe_params->redirect_token = redirect_token;
	setup_params->pipe_params->ft_env = setup_params->ft_env;
	setup_params->pipe_params->env = setup_params->env;
	return (1);
}

int	close_files(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	return (0);
}

static void	setup_child_params(t_heredoc_child_params *child_params,
			int *pipefd, t_heredoc_pipe_params *pipe_params,
			t_shell *shell)
{
	child_params->pipefd = pipefd;
	child_params->delimiter = pipe_params->delimiter;
	child_params->env = shell->env;
	child_params->ft_env = shell->ft_env;
	child_params->quote = has_quotes(pipe_params->delimiter);
}

int	handle_heredoc_pipe_redirect(t_token *lst, t_shell *shell)
{
	int						pipefd[2];
	pid_t					pid1;
	t_heredoc_pipe_params	pipe_params;
	t_heredoc_child_params	child_params;
	t_heredoc_setup_params	setup_params;

	setup_params.ft_env = shell->ft_env;
	setup_params.env = shell->env;
	setup_params.pipe_params = &pipe_params;
	setup_params.gc = &shell->gc;
	if (!setup_heredoc_pipe(lst, &setup_params))
		return (0);
	remove_added_quotes(&pipe_params.delimiter, &shell->gc);
	if (pipe(pipefd) == -1)
		return (0);
	setup_child_params(&child_params, pipefd, &pipe_params, shell);
	pid1 = create_heredoc_child(&child_params, shell);
	if (pid1 == -1)
		return (close_files(pipefd[0], pipefd[1]),
			(free(pipe_params.delimiter), 0));
	pipe_params.pid1 = pid1;
	return (pipe_params.pipefd = pipefd,
		(handle_heredoc_pipe_redirect_part2(&pipe_params, shell)));
}

void	cleanup_heredocs_after_exec(t_pipe_data *data)
{
	int	i;

	if (data->heredocs)
	{
		i = 0;
		while (i < data->hd_count)
		{
			if (data->heredocs[i].fd >= 0)
				close(data->heredocs[i].fd);
			i++;
		}
	}
}
