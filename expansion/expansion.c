/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:15:55 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/10 15:24:41 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	replace_dollar(t_token **t_list)
{
	t_token		*curr;
	char		*temp;

	temp = NULL;
	if (!t_list || !*t_list)
		return ;
	curr = *t_list;
	while (curr)
	{
		if (ft_has_dollar(curr->cmd))
		{
			handle_value(curr->cmd, &temp);
			if (temp)
			{
				free(curr->cmd);
				curr->cmd = ft_strdup(temp);
				free(temp);
			}
			else
			{
				free(curr->cmd);
				curr->cmd = ft_strdup("");
			}
		}
		curr = curr->next;
	}
}

int	find_dollar_index(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '$' && !escape(str, i))
			return (i);
		i++;
	}
	return (-1);
}

int	expand_helper(char **substr, int flag)
{
	int		dollar_index;
	char	*temp;

	dollar_index = find_dollar_index(*substr);
	if (quote_type(*substr) == 0 || quote_type(*substr) == 2)
	{
		if (dollar_index != -1)
		{
			if (dollar_index > 0 && (*substr)[dollar_index - 1] == '\"'
			&& (*substr)[dollar_index + 1] == '\"')
			{
				free(*substr);
				*substr = ft_strdup("\"$\"");
			}
			else
			{
				temp = handle_dollar(*substr, flag);
				free(*substr);
				*substr = temp;
			}
		}
	}
	return (1);
}
