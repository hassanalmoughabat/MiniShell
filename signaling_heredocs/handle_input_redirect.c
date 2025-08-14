/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:00:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/08 01:45:07 by njoudieh42       ###   ########.fr       */
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
			remove_added_quotes(&last_in->next->cmd, NULL);
			if (fd != -1)
				close(fd);
			fd = open(last_in->next->cmd, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(last_in->next->cmd, 2);
				ft_putstr_fd(": ", 2);
				ft_putendl_fd(strerror(errno), 2);
				return (shell->env->exit_status = 1, -2);
			}
		}
		curr = curr->next;
	}
	return (fd);
}

void	error_dup(t_shell *shell)
{
	ft_putstr_fd("minishell: dup2: ", 2);
	ft_putendl_fd(strerror(errno), 2);
	shell->env->exit_status = 1;
	return ;
}

int	handle_less(char *filename, t_shell *shell)
{
	int		fd;
	t_token	*cmd_tokens;
	int		standalone_result;

	standalone_result = handle_standalone_less(filename, shell);
	if (standalone_result != -1)
		return (standalone_result);
	fd = open_last_input_redir(shell);
	if (fd == -2)
		return (shell->env->exit_status = 1, 1);
	if (fd == -1)
		return (0);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (error_dup(shell), close(fd), 1);
	close(fd);
	cmd_tokens = filter_cmd_tokens(shell->tk);
	if (cmd_tokens && cmd_tokens->cmd)
		exec_filtered_cmd(shell, cmd_tokens);
	return (1);
}
