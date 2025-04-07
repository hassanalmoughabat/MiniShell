/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_setter_getter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:31:49 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/06 23:03:29 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

char	*ft_strchr_unescaped(char *str, char c)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			if (i == 0 || str[i - 1] != '\\')
				return (&str[i]);
		}
		i++;
	}
	return (NULL);
}

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

			len = first_quote - equal_pos - 1;
			new_value = malloc(len + 3);
			if (!new_value)
				return ;
			new_value[0] = quote;
			ft_strlcpy(new_value + 1, equal_pos, len + 1);
			new_value[len + 1] = quote;
			new_value[len + 2] = '\0';
		}
		else
			new_value = ft_strdup(equal_pos);
		*value = ft_strdup(new_value);
		free(new_value);
	}
}

char	*get_value(char *input, t_env *env, char quote, int flag)
{
	char	*value;
	char	*result;

	// result = ft_strdup("");
	value = ft_strdup("");
	if (!input)
		return (NULL);
	set_value(&value, quote, input, flag);
	// ft_printf("%s value in get value\n", value);
	if (!value)
		return (NULL);
	handle_value(value, &result, env);
	if (remove_added_quotes(&result) == -1)
		return (free(value), NULL);
	return(result);
	// set_value(&value, quote, input, flag);
	// if (!result)
	// 	return (free(result), free(value), NULL);
	// return (free(value), result);
}

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
		*result = temp;
	}
	return ;
}

char	*get_key(char *input, t_env *env, char *quote, int *ind)
{
	char	*result;
	char	*key;

	result = ft_strdup("");
	if (!input)
		return (NULL);
	set_key(input, &key, quote, ind);
	if (!check_valid_key(key))
		return (free(key), free(result), NULL);
	handle_value(key, &result, env);
	if (remove_added_quotes(&result) == -1)
		return (free(key), free(result), NULL);
	if (!check_key_after_expansion(result))
		return (free(key), free(result), NULL);
	return (free(key), result);
}

int	set_key_value(char *input, char **key, char **value, t_env *env)
{
	if (has_equal(input))
	{
		if (equal_handler_export(input, key, value, env) == -1)
			return (-1);
		return (1);
	}
	ft_printf("above duplicate\n");
	*key = ft_strdup(input);
	ft_printf("key is  %s \n", *key);
	if (*key && !check_valid_key(*key))
	{
		ft_printf("Error in key syntax\n");
		return (free(*key), -1);
	}
	if (remove_added_quotes(key) == -1 || !ft_strcmp(*key, ""))
		return (error_message_export(input, key, value), -1);
	if (*key[0] == '#')
	{
		*key = NULL;
		*value = NULL;
		return (0);
	}
	return (1);
}
