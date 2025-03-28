/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:35:30 by hal-moug          #+#    #+#             */
/*   Updated: 2025/03/16 20:32:39 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i ++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_list_size(t_token *token)
{
	t_token	*current;
	int		size;

	size = 0;
	current = token;
	while (current)
	{
		size ++;
		current = current->next;
	}
	return (size);
}

void	print_env(char **ftenv)
{
	int	i;

	i = 0;
	while (ftenv[i])
	{
		ft_printf("envp is %s\n", ftenv[i]);
		i ++;
	}
}

void	display_list(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		printf("line is %s\n", curr->cmd);
		printf("line is %d\n", curr->type);
		curr = curr->next;
	}
}

void	display_token_list(int size, t_token *head)
{
	t_token	*inp;
	int		i;

	inp = head;
	i = 1;
	while (inp && i <= size)
	{
		ft_printf("\n%s at array of size %d\n", inp->cmd, i);
		inp = inp->next;
		i ++;
	}
}
