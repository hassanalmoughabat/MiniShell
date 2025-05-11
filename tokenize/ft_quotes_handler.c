/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 01:26:28 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/20 22:51:27 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

bool	escape(const char *str, int index)
{
	int	count;

	count = 0;
	while (index > 0 && str[index - 1] == '\\')
	{
		count ++;
		index --;
	}
	return ((count % 2) != 0);
}

int	ft_find_next_quote(char *line, size_t *i)
{
	char	quote_saved;
	size_t	start;

	quote_saved = line[*i];
	start = *i + 1;

	while (line[start])
	{
		if (line[start] == quote_saved && !escape(line, start))
		{
			*i = start + 1;
			return (1);
		}
		start++;
	}
	return (0);
}


int	ft_handle_quotes(char **input, t_token **token_list)
{
	size_t	i;
	char	*temp;
	char	*cmd;
	t_token	*new_token;

	temp = *input;
	i = 0;
	while (temp[i] && !ft_delimeter(temp + i))
	{
		if (ft_check_quotes(temp[i]))
		{
			if (!ft_find_next_quote(temp, &i))
				return (ft_quote_error(temp[i]), 0);
		}
		else
			i++;
	}
	cmd = ft_substr(temp, 0, i);
	if (!cmd)
		return (0);
	new_token = create_token (cmd, T_IDENTIFIER);
	if (!new_token)
		return (free(cmd), 0);
	*input += i;
	return ((ft_add_token_last(token_list, new_token), 1));
}
