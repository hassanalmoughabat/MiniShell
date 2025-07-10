/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/04 17:53:40 by njoudieh42       ###   ########.fr       */
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

int	equal_handler_export(t_token *tk, char *input, char **key,
		char **value, t_env *env)
{
	char	*temp;
	char	quote;
	int		indicator;

	indicator = 0;
	quote = '\0';
	if (input[0] == '=' || input[0] == '-')
	{
		error_validity_export(input, tk);
		return (env->exit_status = 1, -1);
	}
	temp = get_key(tk, input, env, &quote, &indicator);
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

bool contains_quote(const char *str)
{
    if (!str)
        return false;
    while (*str)
    {
        if (*str == '\'' || *str == '"')
            return true;
        str++;
    }
    return false;
}

char *escape_all_quotes(const char *input)
{
	size_t len = ft_strlen(input);
    size_t quote_count = 0;
	size_t	i;
	int	j;

	i = 0;
	j = 0;
    if (!input)
        return (NULL);
    while (i< len)
    {
        if ((input[i] == '\'' || input[i] == '"') && !escape(input, i))
            quote_count++;
		i ++;
    }
	i = 0;
    char *escaped = malloc(len + quote_count + 1);
    if (!escaped)
        return (NULL);
    while (i< len)
    {
        if (input[i] == '\'' || input[i] == '"')
            escaped[j++] = '\\';
        escaped[j++] = input[i++];
    }
    escaped[j] = '\0';
    return (escaped);
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
	if (print_export_env(token, *copy, 0))
		return ;
	curr = curr->next;
	while (curr && curr->cmd)
	{
		i = set_key_value(curr, &key, &value, *copy);
		if (contains_quote(value))
			value = escape_all_quotes(value);
		if ((!key || key[0] == '\0' || !ft_strcmp(key, ""))
			&& !curr->next && i == 1)
			print_export_env(token, *copy, 1);
		else if (i == 1)
		{
			if (has_equal(curr->cmd))
				respective_addition(copy, key, value, 1);
			else
				respective_addition(copy, key, value, 2);
		}
		curr = curr->next;
	}
}
