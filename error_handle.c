/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:24:55 by hal-moug          #+#    #+#             */
/*   Updated: 2025/01/16 11:31:14 by njoudieh         ###   ########.fr       */
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
	ft_printf("%s \n", str);
	ft_printf("exit code : %d\n", error_nb);
}

void	error_print_free(char *str, int error_nb, char **cmd)
{
	ft_printf("%s \n", str);
	ft_printf("exit code : %d\n", error_nb);
	ft_free_tab(cmd);
	exit(1);
}
