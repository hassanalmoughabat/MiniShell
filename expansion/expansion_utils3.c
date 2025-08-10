/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:38:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 16:38:37 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	process_token(t_token *curr, t_shell *shell)
{
	char	*temp;

	if (!ft_has_dollar(curr->cmd))
		return ;
	temp = expand_token_value(curr->cmd, shell);
	free(curr->cmd);
	if (temp)
	{
		curr->cmd = ft_strdup(temp);
		free(temp);
	}
	else
		curr->cmd = ft_strdup("");
}

void	replace_dollar(t_token **t_list, t_shell *shell)
{
	t_token	*curr;

	if (!t_list || !*t_list)
		return ;
	curr = *t_list;
	while (curr)
	{
		if (curr->prev && ft_strcmp(curr->prev->cmd, "<<") == 0)
		{
			curr = curr->next;
			continue ;
		}
		process_token(curr, shell);
		curr = curr->next;
	}
}
