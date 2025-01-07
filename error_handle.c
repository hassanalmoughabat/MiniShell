/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:24:55 by hal-moug          #+#    #+#             */
/*   Updated: 2025/01/07 14:21:04 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	error_print(char *str, int error_nb)
{
	ft_printf("error is %s; exit code: %d", str, error_nb);
}

void	error_print_free(char *str, int error_nb, char **cmd)
{
	ft_printf("error is %s; exit code: %d\n", str, error_nb);
	ft_free_tab(cmd);
	exit(1);
}
