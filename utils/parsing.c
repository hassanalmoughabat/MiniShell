/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:42 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 16:49:58 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	ft_is_builtin(char *cmd)
{
	char	*temp;

	temp = ft_strdup(cmd);
	remove_added_quotes(&temp);
	if ((ft_strcmp(temp, "cd") == 0))
		return (1);
	else if (ft_strcmp(temp, "echo") == 0)
		return (1);
	else if (ft_strcmp(temp, "pwd") == 0)
		return (1);
	else if (ft_strcmp(temp, "exit") == 0)
		return (1);
	else if (ft_strcmp(temp, "unset") == 0)
		return (1);
	else if (ft_strcmp(temp, "export") == 0)
		return (1);
	else if (ft_strcmp(temp, "env") == 0)
		return (1);
	return (0);
}
