/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 15:20:10 by njoudieh42       ###   ########.fr       */
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

int	equal_handler_export(t_token *tk, char **key, char **value, t_shell *shell)
{
	char	*temp;
	char	quote;
	int		indicator;

	indicator = 0;
	quote = '\0';
	if (shell->curr_cmd [0] == '=' || shell->curr_cmd [0] == '-')
	{
		error_validity_export(shell->curr_cmd, tk);
		return (shell->env->exit_status = 1, -1);
	}
	temp = get_key(tk, shell, &quote, &indicator);
	if (!temp)
		return (-1);
	*key = temp;
	*value = get_value(shell->curr_cmd, quote, indicator, shell);
	if (!*value)
		*value = ft_strdup("\"\"");
	return (1);
}

void	respective_addition(t_shell *shell, int flag, t_env **copy)
{
	char	*val;

	if (flag == 1)
	{
		if (check_if_var_exist(copy, shell->key))
			ft_update_env(copy, shell);
		else
			ft_add_env(copy, shell, 1);
		return ;
	}
	if (flag == 2)
	{
		if (check_if_var_exist(copy, shell->key))
		{
			val = get_value_from_env(shell->key, *copy);
			if (val && ft_strcmp(val, ""))
				ft_update_env(copy, shell);
		}
		else
			ft_add_key_to_env(copy, shell->key);
		return ;
	}
}

bool	contains_quote(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '\'' || *str == '"')
			return (true);
		str++;
	}
	return (false);
}

char	*escaped_copy(char *input, size_t len, size_t quote_count)
{
	size_t	i;
	size_t	j;
	char	*escaped;

	i = 0;
	j = 0;
	escaped = malloc(len + quote_count + 1);
	if (!escaped)
		return (NULL);
	while (i < len)
	{
		if (input[i] == '\'' || input[i] == '"')
			escaped[j++] = '\\';
		escaped[j++] = input[i++];
	}
	escaped[j] = '\0';
	return (escaped);
}

char *escape_all_quotes(char *input)
{
	char	*temp;
	size_t	len;
	size_t	quote_count;
	size_t	i;

	i = 0;
	len = ft_strlen(input);
	quote_count = 0;
	if (!input)
		return (NULL);
	while (i < len)
	{
		if ((input[i] == '\'' || input[i] == '"') && !escape(input, i))
			quote_count++;
		i ++;
	}
	temp = escaped_copy(input, len, quote_count);
	return (temp);
}

void	ft_export(t_shell *shell, t_env **copy)
{
	t_token	*curr;
	int		i;

	curr = shell->tk;
	if (print_export_env(shell, 0, *copy))
		return ;
	curr = curr->next;
	while (curr && curr->cmd)
	{
		i = set_key_value(curr, &(shell->key), &(shell->value), shell);
		if (contains_quote(shell->value))
			shell->value = escape_all_quotes(shell->value);
		if ((!shell->key || shell->key[0] == '\0' || !ft_strcmp(shell->key, ""))
			&& !curr->next && i == 1)
			print_export_env(shell, 1, *copy);
		else if (i == 1)
		{
			if (has_equal(curr->cmd))
				respective_addition(shell, 1, copy);
			else
				respective_addition(shell, 2, copy);
		}
		curr = curr->next;
	}
}
