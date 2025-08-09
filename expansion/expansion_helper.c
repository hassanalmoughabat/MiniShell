/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:20:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/06 15:26:22 by njoudieh42       ###   ########.fr       */
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

static int	handle_special_char(char *key, int *i, char **expanded)
{
	if (ft_check_space(key[*i]))
	{
		*expanded = ft_strjoin_char(*expanded, '$');
		(*i)++;
		return (1);
	}
	if (ft_isdigit(key[*i]))
	{
		(*i)++;
		return (1);
	}
	if (ft_check_exceptions(key, *i))
	{
		(*i)++;
		*expanded = ft_strjoin(*expanded, "\\$\\\\");
		return (1);
	}
	return (0);
}

int	dollar_cases(char *key, int *index, char **expanded, t_shell *shell)
{
	char	*temp;

	if (handle_special_char(key, index, expanded))
		return (1);
	if (key[*index] == '?')
	{
		(*index)++;
		temp = ft_itoa(shell->env->exit_status);
		*expanded = ft_strjoin(*expanded, temp);
		free(temp);
		return (1);
	}
	return (0);
}
