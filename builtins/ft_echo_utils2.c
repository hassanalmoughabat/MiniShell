/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 22:56:48 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/25 13:17:01 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	normalize_helper(char *str, int i, char **result)
{
	int		in_space;
	int		j;

	j = 0;
	in_space = 0;
	while (str[i])
	{
		if (ft_check_space(str[i]))
		{
			if (!in_space)
			{
				(*result)[j++] = ' ';
				in_space = 1;
			}
		}
		else
		{
			(*result)[j++] = str[i];
			in_space = 0;
		}
		i++;
	}
	(*result)[j] = '\0';
	return ;
}

char	*normalize_spaces(char *str)
{
	char	*result;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i] && ft_check_space(str[i]))
		i++;
	normalize_helper(str, i, &result);
	return (result);
}

int	is_quote_token(char *str)
{
	if (!str)
		return (0);
	return ((ft_strlen(str) == 1 && (str[0] == '"'
				|| str[0] == '\''))
		|| (ft_strlen(str) == 3 && str[0] == '"'
			&& str[1] == '\'' && str[2] == '"')
		|| (ft_strlen(str) == 3 && str[0] == '\''
			&& str[1] == '"' && str[2] == '\''));
}

void	handle_n_cases(t_token **tk, int *flag)
{
	char	*content;
	int		n_check;

	while (*tk)
	{
		n_check = check_n_flags((*tk)->cmd);
		if (n_check == 1)
			*flag = 1;
		else if (n_check == 2)
		{
			*flag = 1;
			content = extract_content_after_flags((*tk)->cmd);
			if (content && content[0])
			{
				free((*tk)->cmd);
				(*tk)->cmd = content;
				break ;
			}
			free(content);
		}
		else
			break ;
		(*tk) = (*tk)->next;
	}
	return ;
}
