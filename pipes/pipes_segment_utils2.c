/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_segment_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by hal-moug	       #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	handle_great_redirect(t_token *curr)
{
	char	*filename;
	int		fd;

	filename = curr->next->cmd;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	handle_dgreat_redirect(t_token *curr)
{
	char	*filename;
	int		fd;

	filename = curr->next->cmd;
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd >= 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
