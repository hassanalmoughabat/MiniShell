/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 10:43:26 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	cleanup_heredocs(t_heredoc_info *heredocs, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (heredocs[i].fd >= 0)
			close(heredocs[i].fd);
		i++;
	}
	free(heredocs);
}

int	process_single_heredoc(t_token *curr, t_heredoc_info *heredocs,
			t_shell *shell, int i)
{
	char	*delimiter;
	int		quote;

	delimiter = get_delimeter(curr, &shell->gc);
	if (!delimiter)
		return (-1);
	quote = has_quotes(delimiter);
	remove_added_quotes(&delimiter, &shell->gc);
	heredocs[i].fd = handle_dless(delimiter, shell, quote);
	heredocs[i].position = curr;
	if (heredocs[i].fd < 0)
		return (-1);
	return (0);
}

t_heredoc_info	*process_heredocs_before_pipes(t_token *lst,
		t_shell *shell, int *hd_count)
{
	t_token			*curr;
	t_heredoc_info	*heredocs;
	int				count;
	int				i;

	count = has_heredoc(lst);
	*hd_count = count;
	if (count == 0)
		return (NULL);
	heredocs = malloc(sizeof(t_heredoc_info) * count);
	if (!heredocs)
		return (NULL);
	curr = lst;
	i = 0;
	while (curr && i < count)
	{
		if (curr->type == T_DLESS && curr->next)
		{
			if (process_single_heredoc(curr, heredocs, shell, i) == -1)
				return (cleanup_heredocs(heredocs, i), (NULL));
			i++;
		}
		curr = curr->next;
	}
	return (heredocs);
}

void	find_tokens(t_token *lst, t_token **heredoc_token,
			t_token **pipe_token, t_token **redirect_token)
{
	t_token	*curr;

	*heredoc_token = NULL;
	*pipe_token = NULL;
	*redirect_token = NULL;
	curr = lst;
	while (curr)
	{
		if (curr->type == T_DLESS && !(*heredoc_token))
			*heredoc_token = curr;
		else if (curr->type == T_PIPE && !(*pipe_token))
			*pipe_token = curr;
		else if ((curr->type == T_GREAT || curr->type == T_DGREAT)
			&& *pipe_token)
			*redirect_token = curr;
		curr = curr->next;
	}
}

pid_t	create_heredoc_child(t_heredoc_child_params *params, t_shell *shell)
{
	pid_t	pid1;
	int		heredoc_fd;

	pid1 = fork();
	if (pid1 == -1)
		return (-1);
	if (pid1 == 0)
	{
		ft_set_heredoc_signals();
		close(params->pipefd[0]);
		heredoc_fd = handle_dless(params->delimiter, shell, params->quote);
		if (heredoc_fd < 0)
		{
			close(params->pipefd[1]);
			exit(130);
		}
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
		dup2(params->pipefd[1], STDOUT_FILENO);
		close(params->pipefd[1]);
		execve("/bin/cat", (char *[]){"cat", NULL}, params->ft_env);
		shell->env->exit_status = 127;
		exit(127);
	}
	return (pid1);
}
