/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:15:55 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/06 00:09:20 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	expand_helper(char **substr, t_env *env)
{
	if (quote_type(*substr) == 0 || quote_type(*substr) == 2)
	{
		// if (remove_added_quotes(substr) == -1)
		// 	return (0);
		if (ft_has_dollar(*substr))
			*substr = handle_dollar(*substr, env);
	}
	// else if (quote_type(*substr) == 1)
	// {
	// 	if (remove_added_quotes(substr) == -1)
	// 		return (0);
	// }
	return (1);
}

char	*extract_and_expand(char *input, t_env *env)
{
	int		i;
	char	*expanded;
	char	*substr;

	i = 0;
	expanded = ft_strdup("");
	while (input[i])
	{
		if (ft_check_quotes(input[i]))
			substr = extract_quoted_substring(input, &i);
		else
			substr = extract_unquoted_substring(input, &i);
		if (!substr)
			continue ;
		if (!expand_helper(&substr, env))
			return (free(substr), free(expanded), NULL);
		expanded = ft_strjoin(expanded, substr);
		free(substr);
	}
	return (expanded);
}

char	*handle_double_quotes_expansion(char *str, t_env *env)
{
	char	*result;
	char	*temp;

	result = ft_strdup("");
	// if (remove_added_quotes(&str) == -1)
	// 	return (free(result), NULL);
	if (ft_has_dollar(str))
		temp = handle_dollar(str, env);
	else
		temp = ft_strdup(str);
	free(result);
	result = temp;
	return (result);
}

char	*handle_noquotes_singlequotes(char *str, t_env *env)
{
	char	*result;
	char	*temp;

	result = ft_strdup(str);
	if (ft_has_dollar(str) && quote_type(str) == 0)
	{
		temp = handle_dollar(str, env);
		free(result);
		result = temp;
	}
	// if (quote_type(str) == 1 && remove_added_quotes(&result) == -1)
	// 	return (free(result), NULL);
	return (result);
}

void	handle_value(char *value, char **result, t_env *env)
{
	char	*temp;

	temp = NULL;
	ft_printf("%s\n", value);
	if (quote_type(value) == 3)
		temp = extract_and_expand(value, env);
	else if (quote_type(value) == 2)
		temp = handle_double_quotes_expansion(value, env);
	else if (!quote_type(value) || quote_type(value) == 1)
		temp = handle_noquotes_singlequotes(value, env);
	ft_printf("%s\n", temp);
	if (temp)
	{
		if (*result)
			free(*result);
		*result = temp;
	}
	ft_printf("%i\n",quote_type(*result));
}
