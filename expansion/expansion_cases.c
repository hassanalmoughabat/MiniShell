/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:34:09 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/10 15:24:02 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*handle_dollar(char *key, int flag)
{
	int			i;
	char		*value;
	char		*temp;
	char		*expanded;
	char		*var_name;

	i = 0;
	expanded = ft_strdup("");
	while (key[i])
	{
		if (key[i] == '$' && !escape(key, i))
		{
			if (key[i + 1] == '\0' && !flag)
			{
				temp = ft_strjoin_char(expanded, '$');
				free(expanded);
				expanded = temp;
				break ;
			}
			if (!key[i])
				break ;
			i ++;
			if (dollar_cases(key, &i, &expanded))
				continue ;
			else if (key[i] && !ft_check_space(key[i])
				&& !ft_check_exceptions(key, i))
			{
				var_name = extract_dollar_var(key, &i);
				if (!var_name)
				{
					free(var_name);
					continue ;
				}
				value = get_value_from_env(var_name, g_minishell.env);
				if (value)
				{
					temp = join_env_value(expanded, value);
					free(expanded);
					expanded = temp;
					free (value);
				}
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
	return (expanded);
}
