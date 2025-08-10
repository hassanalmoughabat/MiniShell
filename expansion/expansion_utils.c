/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:52:57 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 14:57:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*handle_dollar(char *key, int flag, t_shell *shell)
{
	int		i;
	char	*expanded;
	int		in_s;
	int		in_d;

	i = -1;
	in_s = 0;
	in_d = 0;
	expanded = ft_strdup("");
	while (key[++i])
	{
		update_quotes(&in_s, &in_d, key, i);
		if (key[i] == '$' && !escape(key, i))
		{
			if (in_s && !escape(key, i - 1))
				expanded = ft_strjoin_char(expanded, key[i]);
			else if (!in_s)
				handle_dollar_core(key, &i, &expanded, shell);
		}
		else
			expanded = ft_strjoin_char(expanded, key[i]);
	}
	(void)flag;
	return (expanded);
}
