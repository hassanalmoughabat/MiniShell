/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_setter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:31:49 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/28 01:41:33 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	set_value(char **value, char quote, char *input, int flag)
{
	char	*equal_pos;
	char	*first_quote;
	char	*new_value;
	size_t 	len;

	len = 0;
	equal_pos = ft_strchr(input, '=');
	equal_pos ++;
	if (ft_check_space(*equal_pos))
		*value = ft_strdup("\"\"");
	else
	{
		if (flag)
		{
			first_quote = ft_strchr(equal_pos, quote);
			len = first_quote - equal_pos;
			new_value = malloc(len + 3);
			if (!new_value)
				return ;
			new_value[0] = quote;
			ft_strlcpy(new_value + 1, equal_pos, len + 1);
			new_value[len + 3] = '\0';
			if ((*first_quote + 1))
				new_value = ft_strjoin(new_value, first_quote);
		}
		else
			new_value = ft_strdup(equal_pos);
		if (*value)
			free(*value);
		*value = ft_strdup(new_value);
		free(new_value);
	}
}

void	set_key(char *input, char **result, char *quote, int *flag)
{
	char	*equal_ptr;
	char	*temp;

	equal_ptr = ft_strchr(input, '=');
	*result = ft_substr(input, 0, equal_ptr - input);
	*flag = has_equal_in_quote(input, quote);
	if (*flag == 1)
	{
		temp = ft_strjoin_char(*result, *quote);
		free(*result);
		*result = ft_strdup(temp);
		free(temp);
	}
	return ;
}

int	set_key_value(t_token *tk, char **key, char **value, t_env *env)
{
	char	*input;

	input = tk->cmd;
	if (has_equal(input) && ft_strlen(input) > 1)
	{
		if (equal_handler_export(tk, input, key, value, env) == -1)
			return (-1);
		return (1);
	}
	*key = ft_strdup(input);
	if (*key && !check_valid_key(tk, *key, env))
		return (free(*key), -1);
	if (remove_added_quotes(key) == -1 || !ft_strcmp(*key, ""))
		return (error_message_export(key), -1);
	if (*key[0] == '#')
	{
		*key = NULL;
		*value = NULL;
		return (0);
	}
	return (1);
}
