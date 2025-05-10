/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_validators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:36:24 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/10 10:15:09 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	check_valid_key(char *key, t_env *env)
{
	int		i;

	i = 1;
	if (!key || !key[0])
		return (0);
	if (key[0] == '#')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_' && !ft_check_quotes(key[0])
		&& key[0] != '\\')
	{
		error_validity_export(key);
		return (env->exit_status = 1, 0);
	}
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && !ft_check_quotes(key[i]))
		{
			error_validity_export(key);
			return (env->exit_status = 1, 0);
		}
		i ++;
	}
	env->exit_status = 0;
	return (1);
}

int	check_key_after_expansion(char *key, t_env *env)
{
	int		i;

	i = 1;
	if (!key || !key[0])
	{
		error_validity_export(key);
		return (env->exit_status = 1, 0);
	}
	if (key[0] == '#')
		return (2);
	if ((!ft_isalpha(key[0]) && key[0] != '_' && key[0] != '\\'
			&& key[0] != '\0') || key[0] == '$' )
		return (0);
	while (key[i])
	{
		if ((!ft_isalnum(key[i]) && key[i] != '_' && !ft_strncmp(&key[i], "", 1)
				&& key[i] != '=') || key[i] == '$' || ft_check_quotes(key[i]))
		{
			error_validity_export(key);
			return (env->exit_status = 1, 0);
		}
		i ++;
	}
	env->exit_status = 0;
	return (1);
}
