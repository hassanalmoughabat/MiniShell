/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:38:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/09 02:39:06 by njoudieh42       ###   ########.fr       */
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

char	*handle_dollar(char *key, int flag, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	expanded = ft_strdup_gc(&shell->gc, "");
	while (key[i])
	{
		if (key[i] == '$' && !escape(key, i))
			process_dollar_char(key, &i, &expanded, shell);
		else
			expanded = join_char_and_free(expanded, key[i], &shell->gc);
		i++;
	}
	(void)flag;
	return (expanded);
}
