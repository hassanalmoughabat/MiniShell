/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:16:58 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:19:34 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	contains_symbols(char *tk, int flag)
{
	int		i;

	i = 0;
	if (flag == 1)
	{
		while (tk[i])
		{
			if ((tk[i] == '/'))
				return (1);
			i ++;
		}
	}
	else if (flag == 2)
	{
		while (tk[i])
		{
			if (tk[i] == '>' || tk[i] == '<' || tk[i] == '|')
				return (1);
			i ++;
		}
	}
	return (0);
}

int	count_split_words(char *cmd)
{
	char	**split;
	int		i;

	split = ft_split(cmd, ' ', NULL);
	if (!split)
		return (0);
	i = 0;
	while (split[i])
		i++;
	ft_free_tab(split);
	return (i);
}

int	count_total_args(t_token *tk)
{
	int	total;

	total = 0;
	while (tk)
	{
		if (ft_strchr(tk->cmd, ' '))
			total += count_split_words(tk->cmd);
		else
			total++;
		tk = tk->next;
	}
	return (total);
}

int	add_split_parts(char **argv, int *i, char **split, int count)
{
	int	j;

	j = 0;
	while (split[j] && *i < count)
	{
		argv[*i] = ft_strdup(split[j]);
		if (!argv[*i])
			return (0);
		(*i)++;
		j++;
	}
	return (1);
}

int	add_token_to_argv(char **argv, int *i, t_token *curr)
{
	argv[*i] = ft_strdup(curr->cmd);
	if (!argv[*i])
		return (0);
	remove_added_quotes(&argv[*i], NULL);
	(*i)++;
	return (1);
}
