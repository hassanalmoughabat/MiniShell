/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:15:55 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/07 04:17:14 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	find_dollar_index(const char *str)
{
	int	i = 0;

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

int	expand_helper(char **substr, t_env *env, int flag)
{
	int 	dollar_index;
	char	*temp;
	
	dollar_index = find_dollar_index(*substr);
	if (quote_type(*substr) == 0 || quote_type(*substr) == 2)
	{
		if (dollar_index != -1)
		{
			if (dollar_index > 0 && (*substr)[dollar_index - 1] == '\"' && (*substr)[dollar_index + 1] == '\"')
			{
				free(*substr);
				*substr = ft_strdup("\"$\"");
			}
			else
			{
				temp = handle_dollar(*substr, env, flag);
				free(*substr);
				*substr = temp;
			}
		}
	}
	return (1);
}

char	*extract_and_expand(char *input, t_env *env)
{
	int		i;
	int		flag;
	char	*expanded;
	char	*substr;

	i = 0;
	flag = 0; 
	expanded = ft_strdup("");
	while (input[i])
	{
		if (ft_check_quotes(input[i]))
			substr = extract_quoted_substring(input, &i);
		else
			substr = extract_unquoted_substring(input, &i, &flag);
		if (!substr)
			continue ;
		expand_helper(&substr, env, flag);
		expanded = ft_strjoin(expanded, substr);
		free(substr);
	}
	return (expanded);
}

// char	*handle_double_quotes_expansion(char *str, t_env *env)
// {
// 	char	*result;
// 	char	*temp;
// 	int		dollar_index;

// 	dollar_index = find_dollar_index(str);
// 	result = ft_strdup("");
// 	// if ()
// 	if (ft_has_dollar(str))
// 		temp = handle_dollar(str,env, 0);
// 	else
// 		temp = ft_strdup(str);
// 	free(result);
// 	result = temp;
// 	return (result);
// }

void	handle_value(char *value, char **result, t_env *env)
{
	char	*temp;

	temp = NULL;
	// if (quote_type(value) == 3)
	// 	temp = extract_and_expand(value, env);
	// else if (quote_type(value) == 2)
	// 	temp = extract_and_expand(value, env);
	// else if (!quote_type(value) || quote_type(value) == 1)
	// 	temp = extract_and_expand(value, env);
	temp = extract_and_expand(value, env);
	if (temp)
	{
		if (*result)
			free(*result);
		*result = temp;
	}
}
