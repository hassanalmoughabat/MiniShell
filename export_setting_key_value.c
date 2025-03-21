/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_setting_key_value.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:38:56 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 16:02:13 by njoudieh42       ###   ########.fr       */
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

	if (str)
		free(str); // Free old string to prevent memory leaks

	return (new_str);
}


char	*has_dollar(char *input, t_env *env)
{
	int		i;
	char	*temp;
	char	*result;
	char	*env_value;

	i = 0;
	temp = NULL;
	while (input[i])
	{
		if (input[i] == '$' && !escape(input, i)) // Detect $
		{
			int j = i + 1;
			while (input[j] && (ft_isalnum(input[j]) || input[j] == '_')) // Extract key
				j++;
			char *key = ft_substr(input, i + 1, j - i - 1);
			env_value = my_getenv(key, transform(env)); // Get value from env
			free(key);
			if (!env_value)
				env_value = ""; // If variable doesn't exist, use empty string
			temp = ft_strjoin(result, env_value); // Append variable value
			free(result);
			result = temp;
			i = j;
		}
		else
		{
			temp = ft_strjoin_char(result, input[i]); // Append character
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}

char	*get_value(char *input,t_env *env)
{
	int		i;
	char	*test;
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
	ft_printf("%s\n",value);
	test = has_dollar(value, env);
	if (test)
	{
		free(value);
		value = ft_strdup(test);
	}
	return (value);
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

void	ft_add_key_to_env(t_env **env, t_env **copy, char *key)
{
	t_env	*new_node;

	if (!key)
		return ;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->line = key;
	ft_push_to_env(env, new_node);
	ft_push_to_env(copy, new_node);
}
