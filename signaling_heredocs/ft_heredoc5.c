/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:09:39 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	is_inside_any_quotes(char *str, int pos)
{
	int	i;
	int	in_single;
	int	in_double;

	if (!str || pos < 0 || str[pos] == '\0')
		return (0);
	i = 0;
	in_single = 0;
	in_double = 0;
	while (i < pos)
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i += 2;
			continue ;
		}
		update_quotes_state(str[i], &in_single, &in_double);
		i++;
	}
	return (in_single || in_double);
}

int	calc_new_len(char *delim)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (delim[i])
	{
		if (delim[i] == '$' && delim[i + 1]
			&& !is_inside_any_quotes((char *)delim, i)
			&& (delim[i + 1] == '$'))
		{
			len += 2;
			i += 2;
		}
		else if (delim[i] == '$' && delim[i + 1]
			&& !is_inside_any_quotes((char *)delim, i)
			&& (delim[i + 1] == '"' || delim[i + 1] == '\''))
			i++;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	fill_new_del(char *new_del, char *delim)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (delim[i])
	{
		if (delim[i] == '$' && delim[i + 1]
			&& !is_inside_any_quotes((char *)delim, i)
			&& (delim[i + 1] == '$'))
		{
			new_del[j++] = '$';
			new_del[j++] = '$';
			i += 2;
		}
		else if (delim[i] == '$' && delim[i + 1]
			&& !is_inside_any_quotes((char *)delim, i)
			&& (delim[i + 1] == '"' || delim[i + 1] == '\''))
			i++;
		else
			new_del[j++] = delim[i++];
	}
	new_del[j] = '\0';
}

char	*dollar_delimeter(char *delim)
{
	int		len;
	char	*new_del;

	if (!delim)
		return (NULL);
	len = calc_new_len(delim);
	new_del = malloc(sizeof(char) * (len + 1));
	if (!new_del)
		return (NULL);
	fill_new_del(new_del, delim);
	return (new_del);
}

char	*get_delimeter(t_token *tk)
{
	t_token	*curr;
	char	*temp;

	if (!tk)
		return (NULL);
	curr = tk;
	while (curr)
	{
		if (curr->cmd && ft_strcmp(curr->cmd, "<<") == 0 && curr->next && curr->next->cmd)
		{
			temp = dollar_delimeter(curr->next->cmd);
			return (temp);
		}
		curr = curr->next;
	}
	return (NULL);
}
