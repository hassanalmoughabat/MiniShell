/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:24:32 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/11 01:20:04 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	handle_n(char *option)
{
	int		i;

	i = 1;
	if (!option)
		return (0);
	ft_skip_added_spaces(&option);
	remove_added_quotes(&option);
	if (option[0] != '-')
		return (0);
	while (option[i])
	{
		if (option[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_token *tk)
{
	t_token	*curr;
	int		flag;
	char	*temp;

	flag = 0;
	curr = tk;
	temp = NULL;
	if (curr && !ft_strcmp((curr)->cmd, "echo"))
		curr = (curr)->next;
	while (curr && handle_n((curr)->cmd))
	{
		flag = 1;
		curr = (curr)->next;
	}
	while (curr)
	{
		if (!curr->cmd)
			return ;
		temp = ft_strdup(curr->cmd);
		if (temp && ft_strcmp(temp, " "))
		{
			remove_added_quotes(&temp);
			ft_putstr_fd(temp, 1);
			ft_putstr_fd(" ", 1);
		}
		if (curr && !ft_strcmp(curr->cmd, "new_line"))
			return ;
		curr = curr->next;
	}
	if (!flag)
		ft_putstr_fd("\n", 1);
	g_minishell.env->exit_status = 0;
}
