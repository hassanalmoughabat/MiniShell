/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_validators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:36:24 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/05 21:55:10 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	check_valid_key(char *key)
{
	int		i;

	i = 1;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_' && !ft_check_quotes(key[0])
		&& key[0] != '\\')
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && !ft_check_quotes(key[i]))
		{
			ft_printf("export: %s : not a valid identifier ", key);
			return (0);
		}
		i ++;
	}
	return (1);
}

int	check_key_after_expansion(char *key)
{
	int		i;

	i = 1;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if ((!ft_isalpha(key[0]) && key[0] != '_' && key[0] != '\\' && key[0] != "")
		|| key[0] == '$')
		return (0);
	while (key[i])
	{
		if ((!ft_isalnum(key[i]) && key[i] != '_' && !ft_strncmp(&key[i], "", 1)
				&& key[i] != '=') || key[i] == '$' || ft_check_quotes(key[i]))
		{
			ft_printf("export: %s : not a valid identifier ", key);
			return (0);
		}
		i ++;
	}
	return (1);
}
