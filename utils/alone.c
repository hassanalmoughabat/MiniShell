/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alone.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:56:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:56:19 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	cleanup_minishell(t_shell *shell, char **transform_result, char *input)
{
	if (input)
		free(input);
	if (transform_result)
		free_array(transform_result);
	if (shell)
	{
		if (shell->path)
			free(shell->path);
		// Environment cleanup handled by garbage collector
		// if (shell->env)
		//	free_env_list(shell->env);
	}
}
