/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirect1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:59:48 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 20:06:48 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

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
