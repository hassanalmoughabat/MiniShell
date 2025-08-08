/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/26 17:25:50 by njoudieh42       ###   ########.fr       */
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
	return (0);
}

int	handle_heredoc_pipe_redirect(t_token *lst, t_shell *shell)
{
	int						pipefd[2];
	pid_t					pid1;
	int						quote;
	t_heredoc_pipe_params	pipe_params;
	t_heredoc_child_params	child_params;

	if (!setup_heredoc_pipe(lst, shell->ft_env, shell->env, &pipe_params))
		return (0);
	quote = has_quotes(pipe_params.delimiter);
	remove_added_quotes(&pipe_params.delimiter);
	if (pipe(pipefd) == -1)
		return ((free(pipe_params.delimiter), 0));
	child_params.pipefd = pipefd;
	child_params.delimiter = pipe_params.delimiter;
	child_params.env = shell->env;
	child_params.ft_env = shell->ft_env;
	child_params.quote = quote;
	pid1 = create_heredoc_child(&child_params, shell);
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
