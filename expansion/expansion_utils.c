/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:52:57 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/06 21:06:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*join_char_and_free(char *expanded, char c)
{
	char	*temp;

	temp = ft_strjoin_char(expanded, c);
	free(expanded);
	return (temp);
}

char	*join_str_and_free(char *expanded, char *str)
{
	char	*temp;

	temp = ft_strjoin(expanded, str);
	free(expanded);
	return (temp);
}

char	*handle_var_expansion(char *expanded, char *key, int *i,
	t_shell *shell)
{
	char	*var_name;
	char	*value;

	var_name = extract_dollar_var(key, i);
	if (!var_name)
	{
		free(var_name);
		return (expanded);
	}
	value = get_value_from_env(var_name, shell->env);
	free(var_name);
	if (value)
	{
		expanded = join_str_and_free(expanded, value);
		free(value);
	}
	return (expanded);
}

int	handle_dollar_in_quotes(char *key, int i, int in_s)
{
	if (in_s && !escape(key, i - 1))
		return (1);
	return (0);
}

int	handle_standalone_dollar_check(char *key, int i)
{
	return (handle_standalone_dollar(key, i + 1));
}
