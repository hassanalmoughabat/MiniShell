/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by claude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	has_heredoc(t_token *lst)
{
	int		i;
	t_token	*curr;

	curr = lst;
	i = 0;
	while (curr)
	{
		if (curr->type == T_DLESS)
			i++;
		curr = curr->next;
	}
	return (i);
}

void	close_all_pipes(int **fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

t_token	*next_pipe(t_token *start)
{
	while (start && start->type != T_PIPE)
		start = start->next;
	return (start);
}

int	count_pipes(t_token *lst)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = lst;
	while (curr)
	{
		if (curr->type == T_PIPE)
			count++;
		curr = curr->next;
	}
	return (count);
}

void	close_pipe_descriptors(int *pipe_fds)
{
	if (pipe_fds)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

int	create_pipes(int ***pipes, int pipe_count, t_gc *gc)
{
	int	i;

	*pipes = ft_malloc(gc, sizeof(int *) * pipe_count);
	if (!*pipes)
		return (-1);
	i = -1;
	while (++i < pipe_count)
	{
		(*pipes)[i] = ft_malloc(gc, sizeof(int) * 2);
		if (!(*pipes)[i])
			return (-1);
		if (pipe((*pipes)[i]) == -1)
		{
			while (i >= 0)
				close_pipe_descriptors((*pipes)[i--]);
			return (-1);
		}
	}
	return (0);
}
