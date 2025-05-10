/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:20:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/09 12:08:10 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	ft_check_exceptions(char *str, int index)
{
	if (str[index] == '\\' && ft_isalnum(str[index + 1]))
		return (1);
	return (0);
}

char	*extract_value(char *str, int *index)
{
	char	*result;
	int		start;
	int		length;
	int		j;

	start = *index;
	length = 0;
	j = 0;
	while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
	{
		(*index)++;
		length ++;
	}
	(*index)--;
	result = malloc(sizeof(char) * length + 1);
	if (!result)
		return (NULL);
	while (j < length)
	{
		result[j] = str[start + j];
		j ++;
	}
	result[j] = '\0';
	return (result);
}

int	dollar_cases(char *key, int *index, char **expanded)
{
	int		entrance;
	char	*temp;

	entrance = 0;
	if (ft_check_space(key[*index]))
	{
		*expanded = ft_strjoin_char(*expanded, '$');
		(*index)++ ;
		entrance = 1;
	}
	else if (ft_isdigit(key[*index]))
	{
		(*index)++ ;
		entrance = 1;
	}
	else if (ft_check_exceptions(key, *index))
	{
		(*index)++ ;
		*expanded = ft_strjoin(*expanded, "\\$\\\\");
		entrance = 1;
	}
	else if (key[*index] == '?')
	{
		(*index)++;
		temp = ft_itoa(g_minishell.env->exit_status);
		*expanded = ft_strjoin(*expanded, temp);
		return (free(temp), 1);
	}
	return (entrance);
}
