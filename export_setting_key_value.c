/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_setting_key_value.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:38:56 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/19 13:23:05 by njoudieh42       ###   ########.fr       */
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

char	*get_value(char *input)
{
	int		i;
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
	return (value);
}

int	check_valid_key(char *key)
{
	int		i;

	i = 0;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_' && !ft_check_quotes(key[0]))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && !ft_check_quotes(key[i]) )
			return (0);
		i ++;
	}
	// if (remove_added_quotes(&temp, 1) == -1)
	// 	return (0);
	return (1);
}
// int	available_quotes(char *input)
// {
// 	int	i;

// 	i = 0;
// 	while (input[i])
// 	{
// 		if (ft_check_quotes(input[i]))
// 			return (1);
// 		i ++;
// 	}
// 	return (0);
// }

int	set_key_value(char *input, char **key, char **value, t_env *env)
{
	int	i;

	i = 0;
	if (has_equal(input))
	{
		i = equal_handler_export(input, key, value, 1);
		return (i);
	}
	*key = ft_strdup(input);
	i = equal_handler_export(input, key, NULL, 3);
	if (i == -1)
	{
		error_message_export(input, key, value);
		return (0);
	}
	if (*key[0] == '#')
	{
		equal_handler_export(input, key, value, 2);
		return (2);
	}
	if (*key && check_if_var_exist(&env, *key))
	{
		// ft_printf("i am heerreee\n");
		ft_update_env(*key, get_value_from_env(*key, env), &env);
	}
		
	return (1);
}

void	ft_add_key_to_env(t_env **env, char *key)
{
	t_env	*new_node;
	char	*new_line;

	if (!key)
		return ;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_line = key;
	ft_push_to_env(env, new_node, new_line);
}