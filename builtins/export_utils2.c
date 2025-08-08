/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/24 16:52:20 by njoudieh42       ###   ########.fr       */
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
