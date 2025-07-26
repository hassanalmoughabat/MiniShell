/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 01:26:28 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 18:25:44 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

bool	escape(const char *str, int index)
{
	int	count;
	int	i;

	count = 0;
	while (index > 0 && str[index - 1] == '\\')
	{
		count ++;
		index --;
	}
	i = count % 2;
	return (i != 0);
}

int	ft_find_next_quote(char *line, size_t *i)
{
	char	quote_saved;
	size_t	start;

	quote_saved = line[*i];
	start = *i + 1;
	while (line[start])
	{
		if (line[start] == '\"' && line[start] == quote_saved
			&& !escape(line, start))
		{
			*i = start + 1;
			return (1);
		}
		else if (line[start] == '\'' && line[start] == quote_saved)
		{
			*i = start + 1;
			return (1);
		}
		start++;
	}
	return (0);
}

int	parse_until_delim(char *temp, size_t *i)
{
	while (temp[*i] && !ft_delimeter(temp + *i))
	{
		if (ft_check_quotes(temp[*i]))
		{
			if (!escape(temp, *i))
			{
				if (!ft_find_next_quote(temp, i))
				{
					ft_error_message_quotes(temp[*i]);
					return (0);
				}
			}
			else
				(*i)++;
		}
		else
			(*i)++;
	}
	return (1);
}

int	ft_handle_quotes(char **input, t_token **token_list)
{
	size_t	i;
	char	*cmd;
	t_token	*new_token;

	i = 0;
	if (!parse_until_delim(*input, &i))
		return (0);
	cmd = ft_substr(*input, 0, i);
	if (!cmd)
		return (0);
	new_token = create_token (cmd, T_IDENTIFIER);
	if (!new_token)
		return (free(cmd), 0);
	*input += i;
	ft_add_token_last(token_list, new_token);
	return (1);
}
