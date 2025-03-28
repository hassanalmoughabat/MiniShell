/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:24:55 by hal-moug          #+#    #+#             */
/*   Updated: 2025/03/18 14:39:06 by njoudieh42       ###   ########.fr       */
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

int	ft_error_message_quotes(char c)
{
	ft_putstr_fd("Error unclosed quotes, close your ", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\n", 2);
	return (-1);
}

void	error_message_export(char *input, char **key, char **value)
{
	ft_printf("minishell: export: `%s': not a valid identifier\n", input);
	*key = NULL;
	*value = NULL;
	return ;
}
