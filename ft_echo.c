/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:24:32 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/06 22:35:06 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

// void	echo_quotes_removal(t_token **tk)
// {
// 	t_token	*current;

// 	if (!*tk || !tk)
// 		return ;
// 	current = *tk;
// 	while (current)
// 	{
// 		remove_added_quotes(&current->cmd);
// 		current = current->next;
// 	}
// }

int	handle_n(char *option)
{
	int		i;

	i = 1;
	if (!option)
		return (0);
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

void	ft_echo(t_token *tk, t_env *env)
{
	t_token	*curr;
	int		flag;
	char	*temp;

	flag = 0;
	curr = tk;
	temp = NULL;
	if (curr && !ft_strcmp(curr->cmd, "echo"))
		curr = curr->next;
	while (curr && handle_n(curr->cmd))
	{
		flag = 1;
		curr = curr->next;
	}
	while (curr)
	{
		if (curr)
		{
			temp = ft_strdup(curr->cmd);
			if (temp)
			{
				remove_added_quotes(&temp);
				ft_printf("%s ", temp);
			}	
		}
		if (curr && !ft_strcmp(curr->cmd, "new_line"))
			return ;
		curr = curr->next;
	}
	if (!flag)
		ft_printf("\n");
}
