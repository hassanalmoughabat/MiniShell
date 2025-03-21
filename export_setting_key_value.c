/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_setting_key_value.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:38:56 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 20:47:02 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

char	*get_key(char *input)
{
	size_t	i;
	char	*key;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i] && input[i] != '=')
		i ++;
	key = ft_substr(input, 0, i);
	if (!check_valid_key(key))
		return (NULL);
	return (key);
}

char	*get_dollar_value(char *str, t_env *env)
{
	char	*env_value;

	if (!str || str[0] != '$')
		return (NULL);
	env_value = my_getenv(str + 1, transform(env));
	if (env_value && str)
		return (ft_strdup(env_value));
	return (NULL);
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
	new_str = (char *)malloc(len + 2); // +1 for new char, +1 for '\0'
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;   // Append the new character
	new_str[i + 1] = '\0'; // Null-terminate
	return (new_str);
}

char	*dollar_handler(char *value, t_env *env, int *index)
{
	char	*result;
	char	*temp = ft_strdup("");  // Initialize temp
	int		i;

	i = *index;
	while (value[i])
	{	
		result = ft_strjoin_char(temp, value[i]);
		free(temp);
		temp = result;
		if (ft_strlen(result) == 1)
		{
			if (expansion_helper(value[i], env))
				return (get_value_from_env(result, env));
		}
		else
		{
			if (get_va_expander(result, env))
				return (get_value_from_env(result, env));
		}
		i++;
	}
	return (NULL);
}


char	*replace_dollars(char *value,t_env *env)
{
	int		i;
	char	*result;
	char	*temp;
	char	*env_value;

	result =NULL;
	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && !escape(value, i))
		{
			i++;
			env_value = dollar_handler(value, env, &i);
			if (!env_value)
				return (result);
			temp = ft_strjoin(result, env_value);
			free(result);
			result = temp;
			i++;
		}
		else
		{
			temp = ft_strjoin_char(result, value[i]);
			// free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}

char	*get_value(char *input, char *key, t_env *env)
{
	int		i;
	// char	*test;
	char	*value;
	size_t	len;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i] && input[i] != '=')
		i++;
	if (ft_check_space(input[i + 1]))
		return (ft_strdup(""));
	len = ft_strlen(input) - i -1;
	value = ft_substr(input, i + 1, len);
	ft_printf("%s %s %s\n",value, key,input);
	remove_added_quotes(&value,0);
	char *expanded_value = replace_dollars(value,env);
	ft_printf("HEREE\n");
	ft_printf("%s\n",expanded_value);
	free(value);
	return (expanded_value);
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

int	check_valid_key(char *key)
{
	int		i;

	i = 0;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_' && !ft_check_quotes(key[0]) && !ft_backslash(key, 0))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && !ft_check_quotes(key[i]) && !ft_backslash(key, i))
			return (0);
		i ++;
	}
	return (1);
}

int	set_key_value(char *input, char **key, char **value,t_env *env)
{
	int		i;
	// size_t	j;
	// size_t	temp;

	i = 0;
	// j = 0;
	// if (has_equal(input) && start_with_quotes(input))
	// {
	// 	temp = j;
	// 	if (ft_find_next_quote(input, &j))
	// 	{
	// 		if (ft_is_empty(input,temp, j - 1))	
	// 	}
	// }
	if (has_equal(input))
	{
		i = (equal_handler_export(input, key, value, 1,env));
		return (i);
	}
	*key = ft_strdup(input);
	if (!check_valid_key(*key))
	{
		ft_printf("Error in key syntax\n");
		return (free(*key),-1);
	}
	i = remove_added_quotes(key, 1);
	if (i == -1 || !ft_strcmp(*key,""))
		return(error_message_export(input, key, value),0);
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
	new_node->line = key;
	ft_push_to_env(copy, new_node);
}
