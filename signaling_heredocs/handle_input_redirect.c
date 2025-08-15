/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:00:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 20:06:42 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	open_input_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	return (fd);
}

int	handle_standalone_less(char *filename, t_shell *shell)
{
	int	fd;

	if (shell->tk && shell->tk->cmd && ft_strcmp(shell->tk->cmd, "<") == 0)
	{
		fd = open_input_file(filename);
		if (fd == -1)
		{
			shell->env->exit_status = 1;
			return (0);
		}
		close(fd);
		return (1);
	}
	return (-1);
}

void	error_out(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	return ;
}

int	open_last_input_redir(t_shell *shell)
{
	t_token	*curr;
	int		fd;
	t_token	*last_in;

	fd = -1;
	curr = shell->tk;
	last_in = NULL;
	while (curr)
	{
		if (curr->type == T_LESS && curr->next && is_valid_filename(curr->next))
		{
			last_in = curr;
			remove_added_quotes(&last_in->next->cmd, &shell->gc);
			if (fd != -1)
				close(fd);
			fd = open(last_in->next->cmd, O_RDONLY);
			if (fd == -1)
			{
				error_out(last_in->next->cmd);
				return (shell->env->exit_status = 1, -2);
			}
		}
		curr = curr->next;
	}
	return (fd);
}
