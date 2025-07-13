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

int	setup_heredoc_pipe(t_token *lst, char **ft_env, t_env *env,
			t_heredoc_pipe_params *pipe_params)
{
	t_token					*heredoc_token;
	t_token					*pipe_token;
	t_token					*redirect_token;
	char					*delimiter;

	find_tokens(lst, &heredoc_token, &pipe_token, &redirect_token);
	if (!heredoc_token || !pipe_token || !redirect_token
		|| !redirect_token->next)
		return (0);
	delimiter = get_delimeter(lst);
	if (!delimiter)
		return (0);
	pipe_params->delimiter = delimiter;
	pipe_params->redirect_token = redirect_token;
	pipe_params->ft_env = ft_env;
	pipe_params->env = env;
	return (1);
}

int	close_files(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}

int	handle_heredoc_pipe_redirect(t_token *lst, char **ft_env, t_env *env)
{
	int						pipefd[2];
	pid_t					pid1;
	int						quote;
	t_heredoc_pipe_params	pipe_params;
	t_heredoc_child_params	child_params;

	if (!setup_heredoc_pipe(lst, ft_env, env, &pipe_params))
		return (0);
	quote = is_delimeter_quoted(lst);
	if (pipe(pipefd) == -1)
		return ((free(pipe_params.delimiter), 0));
	child_params.pipefd = pipefd;
	child_params.delimiter = pipe_params.delimiter;
	child_params.env = env;
	child_params.ft_env = ft_env;
	child_params.quote = quote;
	pid1 = create_heredoc_child(&child_params);
	if (pid1 == -1)
	{
		close_files(pipefd[0], pipefd[1]);
		return ((free(pipe_params.delimiter), 0));
	}
	pipe_params.pid1 = pid1;
	return (pipe_params.pipefd = pipefd,
		(handle_heredoc_pipe_redirect_part2(&pipe_params)));
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
		free(data->heredocs);
	}
}
