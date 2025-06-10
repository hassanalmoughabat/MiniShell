/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:36:57 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/10 17:56:26 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int has_heredoc(t_token *lst)
{
	int i;
	t_token *curr;

	curr = lst;
	i = 0;
	while (curr)
	{
		if (lst->type == T_DLESS)
			i++;
		curr = curr->next;
	}
	return (i);
}

void	handle_pipe(t_token *lst, char **ft_env, t_env *env)
{
printf("");
}

