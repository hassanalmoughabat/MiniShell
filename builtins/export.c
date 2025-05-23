/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/10 20:40:25 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	has_equal(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	equal_handler_export(char *input, char **key, char **value, t_env *env)
{
	char	*temp;
	char	quote;
	int		indicator;

	indicator = 0;
	quote = '\0';
	if (input[0] == '=' || input[0] == '-')
	{
		error_validity_export(input);
		return (env->exit_status = 1, -1);
	}
	temp = get_key(input, env, &quote, &indicator);
	if (!temp)
		return (-1);
	*key = temp;
	*value = get_value(input, quote, indicator);
	if (!*value)
		*value = ft_strdup("\"\"");
	return (1);
}

void	respective_addition(t_env **copy, char *key, char *value, int flag)
{
	char	*val;

	if (flag == 1)
	{
		if (check_if_var_exist(copy, key))
			ft_update_env(key, value, &g_minishell.env, copy);
		else
			ft_add_env(key, value, copy, 1);
		return ;
	}
	if (flag == 2)
	{
		if (check_if_var_exist(copy, key))
		{
			val = get_value_from_env(key, *copy);
			if (val && ft_strcmp(val, ""))
				ft_update_env(key, val, &g_minishell.env, copy);
		}
		else
			ft_add_key_to_env(copy, key);
		return ;
	}
}

void	ft_export(t_token *token, t_env **copy)
{
	t_token	*curr;
	char	*key;
	char	*value;
	int		i;

	curr = token;
	key = NULL;
	value = NULL;
	if (print_export_env(token, *copy))
		return ;
	curr = curr->next;
	while (curr && curr->cmd)
	{
		i = set_key_value(curr->cmd, &key, &value, *copy);
		if (i == 1)
		{
			if (has_equal(curr->cmd))
				respective_addition(copy, key, value, 1);
			else
				respective_addition(copy, key, value, 2);
		}
		if (i == 2 || !ft_strcmp(curr->cmd, "new_line"))
			return ;
		curr = curr->next;
	}
}
