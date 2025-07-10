/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:34:09 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/30 15:15:43 by njoudieh42       ###   ########.fr       */
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

char *handle_dollar(char *key, int flag) 
{
    int     i;
    char    *value;
    char    *temp;
    char    *expanded;
    char    *var_name;
    int     in_single_quotes;
    int     in_double_quotes;

    i = 0;
    in_single_quotes = 0;
    in_double_quotes = 0;
    expanded = ft_strdup(""); 
    while (key[i])
    {
        if (key[i] == '\'' && !escape(key, i) && !in_double_quotes)
            in_single_quotes = !in_single_quotes;
        else if (key[i] == '"' && !escape(key, i) && !in_single_quotes)
            in_double_quotes = !in_double_quotes;
        if (key[i] == '$' && !escape(key, i))
        {
            if (in_single_quotes && !escape(key, i - 1))
            {
                temp = ft_strjoin_char(expanded, key[i]);
                free(expanded);
                expanded = temp;
                i++;
                continue;
            }
            if (!in_single_quotes)
            {
                if (handle_standalone_dollar(key, i + 1))
                {
                    temp = ft_strjoin_char(expanded, '$');
                    free(expanded);
                    expanded = temp;
                    i++;
                    continue;
                }
                i++;
                if (dollar_cases(key, &i, &expanded))
                    continue;
                else if (key[i] && !ft_check_space(key[i]) 
                    && !ft_check_exceptions(key, i))
                {
                    var_name = extract_dollar_var(key, &i);
                    if (!var_name)
                    {
                        free(var_name);
                        continue;
                    }
                    value = get_value_from_env(var_name, g_minishell.env);
                    free(var_name);
                    if (value)
                    {
                        temp = ft_strjoin(expanded, value);
                        g_minishell.env->safe_quotes = true;
                        free(expanded);
                        expanded = temp;
                        free(value);
                    }
                }
            }
            else
            {
                temp = ft_strjoin_char(expanded, key[i]);
                free(expanded);
                expanded = temp;
            }
        }
        else
        {
            temp = ft_strjoin_char(expanded, key[i]);
            free(expanded);
            expanded = temp;
		}
	i++;
	}
	(void)flag;
	return (expanded);
}
