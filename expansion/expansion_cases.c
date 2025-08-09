/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:34:09 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/09 02:38:20 by njoudieh42       ###   ########.fr       */
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

int	process_dollar_cases(char *key, int *i, char **expanded,
	t_shell *shell)
{
	if (dollar_cases(key, i, expanded, shell))
		return (1);
	if (key[*i] && !ft_check_space(key[*i]) && !ft_check_exceptions(key, *i))
	{
		*expanded = handle_var_expansion(*expanded, key, i, shell);
	}
	return (0);
}

void	process_dollar_not_in_single_quotes(char *key, int *i,
	char **expanded, t_shell *shell)
{
	if (handle_standalone_dollar_check(key, *i))
	{
		*expanded = join_char_and_free(*expanded, '$');
		(*i)++;
		return ;
	}
	(*i)++;
	process_dollar_cases(key, i, expanded, shell);
}

void	process_dollar_char(char *key, int *i, char **expanded,
	t_shell *shell)
{
	int	in_s;
	int	in_d;

	in_s = 0;
	in_d = 0;
	update_quotes(&in_s, &in_d, key, *i);
	if (handle_dollar_in_quotes(key, *i, in_s))
	{
		*expanded = join_char_and_free(*expanded, key[*i]);
		return ;
	}
	if (!in_s)
	{
		process_dollar_not_in_single_quotes(key, i, expanded, shell);
		(*i)--;
	}
	else
		*expanded = join_char_and_free(*expanded, key[*i]);
}
