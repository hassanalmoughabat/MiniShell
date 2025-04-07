/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:20:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/07 04:15:09 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

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
	result = malloc(sizeof(char) * length);
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
	// else 			// if (key[i] == ?)
	// // {
	// // 	var_name = 
	// // }
	return (entrance);
}

char	*handle_dollar(char *key, t_env *env, int flag)
{
	int			i;
	char		*value;
	char		*expanded;
	char		*var_name;

	i = 0;
	expanded = ft_strdup("");
	while (key[i])
	{
		if (key[i] == '$' && !escape(key, i))
		{
			if (!key[i + 1] && !flag)
			{
				expanded = ft_strjoin_char(expanded, '$');
				if (!key[i + 1])
					break ;
			}
			i ++;
			if (!key[i])
				break;
			if (dollar_cases(key, &i, &expanded))
				continue ;
			else if (key[i] && !ft_check_space(key[i])
				&& !ft_check_exceptions(key, i))
			{
				var_name = extract_dollar_var(key, &i);
				if (!var_name)
				{
					free(var_name);
					continue;
				}
				value = get_value_from_env(var_name, env);
				if (value)
					expanded = join_env_value(expanded, value);
				free(var_name);
			}
		}
		else
			expanded = ft_strjoin_char(expanded, key[i]);
		i++;
	}
	return (expanded);
}
