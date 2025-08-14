/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:24:32 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/24 23:08:04 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	is_valid_n_flag(char *word)
{
	int	i;

	i = 1;
	if (!word || word[0] != '-' || word[1] == '\0')
		return (0);
	while (word[i] && word[i] == 'n')
		i++;
	return (word[i] == '\0');
}

void	print_echo(char **temp, t_gc *gc)
{
	remove_added_quotes(temp, gc);
	if (temp && ft_strcmp(*temp, ""))
	{
		ft_putstr_fd(*temp, 1);
		ft_putstr_fd(" ", 1);
	}
	return ;
}

void	ft_echo(t_shell *shell)
{
	t_token	*curr;
	int		flag;
	char	*temp;

	flag = 0;
	curr = shell->tk;
	if (curr && !ft_strcmp(curr->cmd, "echo"))
		curr = curr->next;
	handle_n_cases(&curr, &flag, &shell->gc);
	while (curr)
	{
		if (!curr->cmd)
			return ;
		temp = ft_strdup_gc(&shell->gc, curr->cmd);
		if (temp && ft_strcmp(temp, " "))
			print_echo(&temp, &shell->gc);
		curr = curr->next;
	}
	if (!flag)
		ft_putstr_fd("\n", 1);
	shell->env->exit_status = 0;
}
