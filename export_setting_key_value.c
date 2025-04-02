/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_setting_key_value.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:38:56 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/03 00:58:10 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

bool	contains_mixed_quotes(char *str , int in_single, int in_double)
{
	bool	has_single;
	bool	has_double;
	int		i;

	i = 0;
	has_double = false;
	has_single = false;
    while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			has_single = true;
			in_single = !in_single;
		}
		else if (str[i] == '"' && !in_single && !escape(str, i))
		{
			has_double = true;
			in_double = !in_double;
		}
		i ++;
	}
	if (has_double && has_single)
		return (true);
	return (false);
}
bool detect_Quote(char *str, char quote)
{
	char	*first_occ;
	char	*second_occ;

	first_occ = ft_strchr(str, quote);
	if (!first_occ)
		return (false);
	second_occ = ft_strchr(first_occ + 1, quote);
	if (second_occ)
		return (true);
	return (false);
}
int		quote_type(char *str)
{
	if (!str || !str[0])
		return (0);
	if (contains_mixed_quotes(str, 0, 0))
		return (3);
	if (detect_Quote(str, '\''))
		return (1);
	if (detect_Quote(str,'"'))
		return (2);
	return (0);
}

int		ft_check_dollar(char *value, int index)
{
	if (value[index] == '$' && !escape(value, index))
		return (1);
	return (0);
}
int ft_has_dollar(char *str)
{
	int		index;

	index = 0;
	while (str[index])
	{
		if (str[index] == '$' && !escape(str, index))
			return (1);
		index ++;
	}
	return (0);	
}

int	check_valid_key(char *key)
{
	int		i;

	i = 1;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_' && !ft_check_quotes(key[0]) && key[0] != '\\' && !ft_has_dollar(key))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && !ft_check_quotes(key[i]) && !ft_has_dollar(key) && key[i] != '=')
		{
			ft_printf("export: %s : not a valid identifier ",key);
			return (0);
		}
		i ++;
	}
	return (1);
}

char	*extract_dollar_var(char *key, int *index)
{
	char	*dollar_value;
	int		start;
	int		length;
	int		j;

	start = *index;
	length = 0;
	j = 0;
	while (key[*index] && !ft_check_space(key[*index]) &&
		key[*index] != '$' && !ft_check_quotes(key[*index]))
	{
		(*index) ++;
		length ++;
	}
	if (ft_check_dollar(key, *index) || ft_check_space(key[*index])
		|| ft_check_quotes(key[*index]))
		(*index) --;
	dollar_value = malloc(sizeof(char) * (length + 1));
	if (!dollar_value)
		return (NULL);
	while (j < length)
	{
		dollar_value[j] = key[start + j];
		j++;
	}
	dollar_value[j] = '\0';
	return (dollar_value);
}

char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}
char	*join_env_value(char *expanded, char *value)
{
	char *temp;
	temp = expanded;
	expanded = ft_strjoin(expanded, value);
	free(temp);
	return expanded;
}

int		ft_check_exceptions(char *str, int index)
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
	while (str[*index] && !ft_check_space(str[*index]))
	{
		(*index) ++;
		length ++;
	}
	if (ft_check_space(str[*index]))
		(*index) --;
	result = malloc(sizeof(char) * length + 1);
	if (!result)
		return (NULL);
	while (j < length)
	{
		result[j] = str[start + j];
		j ++;
	}
	result[j] ='\0';
	return (result);
}
char	*handle_dollar(char *key, t_env *env)
{
	int		 i = 0;
	char	*value;
	char	*expanded;
	char	*var_name;

	expanded = ft_strdup("");
	while (key[i])
	{
		if (key[i] == '$' && !escape(key, i))
		{
			if (key[i + 1] && !ft_check_space(key[i + 1]) && !ft_check_exceptions(key, i + 1))
			{
				if (!key[i + 1] || ft_check_space(key[i + 1]))
				{
					expanded = ft_strjoin(expanded, "\\$");
					i++;
					continue;
				}
				if (ft_isdigit(key[i]))
				{
					i += 2;
					continue;
				}
				// if (key[i] == ?)
				// {
				// 	var_name = 
				// }
				var_name = extract_dollar_var(key, &i);
				if (var_name)
				{
					value = get_value_from_env(var_name, env);
					if (value)
						expanded = join_env_value(expanded, value);
					free(var_name);
				}
			}
			else if (ft_check_exceptions(key, i + 1))
			{
				i += 2;
				var_name = ft_strjoin("\\$\\\\", extract_value(key, &i));
				expanded = ft_strjoin(expanded, var_name);
				free (var_name);
			}
		}
		else
			expanded = ft_strjoin_char(expanded, key[i]);
		i++;
	}
	return (expanded);
}

int	check_key_after_expansion(char *key)
{
	int		i;

	i = 1;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if ((!ft_isalpha(key[0]) && key[0] != '_' && key[0] != '\\' && key[0] != "")
		|| key[0] == '$')
		return (0);
	while (key[i])
	{
		if ((!ft_isalnum(key[i]) && key[i] != '_' && !ft_strncmp(&key[i], "", 1) && key[i] != '=')
			|| key[i] == '$' || ft_check_quotes(key[i]))
		{
			ft_printf("export: %s : not a valid identifier ",key);
			return (0);
		}
		i ++;
	}
	return (1);
}
char	*extract_quoted_substring(char *input, int *i)
{
	char	quote;
	int		start;
	char	*substr;

	quote = input[*i];
	start = (*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	substr = ft_substr(input, start, (*i) - start);
	return (substr);
}
char	*extract_unquoted_substring(char *input, int *i)
{
	int		start;
	char	*substr;

	start = *i;
	while (input[*i] && !ft_check_quotes(input[*i]))
		(*i)++;
	substr = ft_substr(input, start, (*i) - start);
	return (substr);
}
char	*extract_and_expand(char *input, t_env *env)
{
	int		i;
	char	*temp;
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
			continue;
		if (quote_type(substr) == 0 || quote_type(substr) == 2)
		{
			if (remove_added_quotes(&substr, 1) == -1)
				return (free(substr), free(expanded), NULL);
			if (ft_has_dollar(substr))
				substr = handle_dollar(substr, env);
		}
		else if (quote_type(substr) == 1)
		{
			if (remove_added_quotes(&substr, 1) == -1)
				return (free(substr), free(expanded), NULL);
		}
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
	if (remove_added_quotes(&str, 1) == -1)
		return (free(result), NULL);
	if (ft_has_dollar(str))
		temp = handle_dollar(str, env);
	else
		temp = ft_strdup(str);
	free(result);
	result = temp;
	return (result);
}


char	*handle_Noquotes_Singlequotes(char *str, t_env *env)
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
	if (quote_type(str) == 1 && remove_added_quotes(&result, 1) == -1)
		return (free(result), NULL);
	return (result);
}
void	handle_value(char *value, char **result, t_env *env)
{
	char *temp;

	temp = NULL;
	if (quote_type(value) == 3)
		temp = extract_and_expand(value, env);
	else if (quote_type(value) == 2)
		temp = handle_double_quotes_expansion(value, env);
	else if (!quote_type(value) || quote_type(value) == 1)
		temp = handle_Noquotes_Singlequotes(value, env);
	if (temp)
	{
		if (*result)
			free(*result);
		*result = temp;
	}
}
char	*ft_strcat(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*ptr)
		ptr++;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

void	set_value(char **value, char quote, char *input, int flag)
{
	char	*equal_pos;
	char	*first_quote;
	char	*new_value;

	equal_pos = ft_strchr(input, '=');
	equal_pos ++;
	if (ft_check_space(*equal_pos))
		*value =ft_strdup("\"\"");
	else
	{
		if (flag)
		{
			first_quote = ft_strchr(equal_pos, quote);
			new_value = malloc(ft_strlen(equal_pos));
			if (!new_value)
				return;
			ft_strlcpy(new_value, equal_pos, first_quote - equal_pos + 1);
			ft_strcat(new_value, first_quote + 1);
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
	char 	*result;

	result = ft_strdup("");
	value = ft_strdup("");
	if (!input)
		return (NULL);
	set_value(&value, quote, input, flag);
	if (!value)
		return (NULL);
	handle_value(value, &result, env);
	set_value(&value, quote, input, flag);
	if (!result)
		return (free(result), free(value), NULL);
	return (result);
}

int		has_equal_in_quote(char *input, char *quote)
{
	int		i;
	int		j;
	char	*substr;

	i = 0;
	while(input[i])
	{
		if (ft_check_quotes(input[i]) && !escape(input, i))
		{
			*quote = input[i];
			ft_printf("%s",quote);
			j = ++ i;
			while (input[i] && (input[i] != *quote || escape(input, i)))
				i++;
			substr = ft_substr(input, j, i - j);
			if (has_equal(substr))
				return (free(substr), 1);
			free(substr);
		}
		i ++ ;
	}
	return (0);
}

void	set_key(char *input,char **result, char *quote, int *flag)
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
bool	multi_equal(char *str)
{
	char	*first_equal;

	first_equal = ft_strchr(str, '=');
	if (first_equal)
		return (true);
	return (false);
}
char	*get_key(char *input , t_env *env, char *quote, int *ind)
{
	char	*result;
	char	*temp;
	char	*new_result;
	char	*key;

	result = ft_strdup("");
	if (!input)
		return (NULL);
	set_key(input, &key, quote, ind);
	if (!check_valid_key(key))
		return (free(key), free(result), NULL);
	handle_value(key, &result, env);
	if (multi_equal(result))
	{
    	temp = ft_strchr(result, '=');
    	new_result = ft_substr(result, 0, temp - result);
    	free(result);
		result = ft_strdup(new_result);
		free(new_result);
	}
	if (!check_key_after_expansion(result))
		return (free(key), free(result), NULL);
	return (free(key), result);
}

int	ft_backslash(char *key, int index)
{
	if ( key[index] == '\\')
	{
		if (key[index + 1] == '\\')
			return (1);
		return (0);
	}
	return (0);
}

int	set_key_value(char *input, char **key, char **value,t_env *env)
{
	if (has_equal(input))
	{
		if (equal_handler_export(input, key, value, 1, env) == -1)
			return (-1);
		return (1);
	}
	*key = ft_strdup(input);
	if (!check_valid_key(*key))
	{
		ft_printf("Error in key syntax\n");
		return (free(*key),-1);
	}
	if (remove_added_quotes(key, 1) == -1 || !ft_strcmp(*key,""))
		return(error_message_export(input, key, value),-1);
	if (*key[0] == '#')
		return(equal_handler_export(input, key, value, 2,env),2);
	return (1);
}

void	ft_add_key_to_env(t_env **copy, char *key)
{
	t_env	*new_node;

	if (!key)
		return ;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->line = ft_strdup(key);
	new_node->next = NULL;
	ft_push_to_env(copy, new_node);
}

