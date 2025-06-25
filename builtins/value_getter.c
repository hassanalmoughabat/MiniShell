/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_getter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:16:46 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/28 01:37:56 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	has_equal_in_quote(char *input, char *quote)
{
	int		i;
	int		j;
	char	*substr;

	i = 0;
	while (input[i])
	{
		if (ft_check_quotes(input[i]) && !escape(input, i))
		{
			*quote = input[i];
			j = ++ i;
			while (input[i] && (input[i] != *quote || escape(input, i)))
				i++;
			substr = ft_substr(input, j, i - j);
			if (has_equal(substr))
				return (free(substr), 1);
			free(substr);
		}
		i ++;
	}
	return (0);
}

char	*get_value(char *input, char quote, int flag)
{
	char	*value;
	char	*result;

	value = ft_strdup("");
	if (!input)
		return (NULL);
	set_value(&value, quote, input, flag);
	if (!value)
		return (NULL);
	handle_value(value, &result);
	if (remove_added_quotes(&result) == -1)
		return (free(value), NULL);
	return (result);
}

char	*get_key(t_token *tk, char *input, t_env *env, char *quote, int *ind)
{
	char	*result;
	char	*key;

	result = ft_strdup("");
	if (!input)
		return (NULL);
	set_key(input, &key, quote, ind);
	if (!check_valid_key(tk, key, env))
		return (free(key), free(result), NULL);
	handle_value(key, &result);
	if (remove_added_quotes(&result) == -1)
		return (free(key), free(result), NULL);
	if (!check_key_after_expansion(result, env, tk))
		return (error_message_export(&result), NULL);
	return (result);
}
