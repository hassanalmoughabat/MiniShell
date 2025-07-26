/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:34:09 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 14:53:26 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	handle_standalone_dollar(char *key, int i)
{
	if (key[i] == '\0' || key[i] == ' ' || key[i] == '\t'
		|| key[i] == '\n' || key[i] == '$' || key[i] == '='
		|| key[i] == ':' || key[i] == '/' || key[i] == '+'
		|| key[i] == '-' || key[i] == ',' || key[i] == '.')
		return (1);
	return (0);
}

void	update_quotes(int *s_quotes, int *d_quotes, char *key, int i)
{
	if (key[i] == '\'' && !escape(key, i) && !*d_quotes)
		*s_quotes = !*s_quotes;
	else if (key[i] == '"' && !escape(key, i) && !*s_quotes)
		*d_quotes = !*d_quotes;
	return ;
}

void	append_value(char **expanded, char *value)
{
	char	*temp;

	if (!value)
		return ;
	temp = ft_strjoin(*expanded, value);
	free(*expanded);
	*expanded = temp;
	free(value);
}

int	process_var(char *key, int *i, char **expanded, t_shell *shell)
{
	char	*var_name;
	char	*value;

	var_name = extract_dollar_var(key, i);
	if (!var_name)
		return (0);
	value = get_value_from_env(var_name, shell->env);
	free(var_name);
	if (value)
		append_value(expanded, value);
	return (1);
}

int	handle_dollar_core(char *key, int *i, char **expanded, t_shell *shell)
{
	if (handle_standalone_dollar(key, *i + 1))
	{
		*expanded = ft_strjoin_char(*expanded, '$');
		(*i)++;
		return (1);
	}
	(*i)++;
	if (dollar_cases(key, i, expanded, shell))
		return (1);
	if (key[*i] && !ft_check_space(key[*i]) && !ft_check_exceptions(key, *i))
		return (process_var(key, i, expanded, shell));
	return (0);
}
