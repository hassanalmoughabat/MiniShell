/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:54:58 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 19:15:09 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	redirect_last_out(t_token *last, t_shell *shell)
{
	int	flags;
	int	fd;
	int	dup_result;

	remove_added_quotes(&last->next->cmd);
	if (last->type == T_GREAT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(last->next->cmd, flags, 0644);
	if (fd == -1)
	{
		shell->env->exit_status = 1;
		return (0);
	}
	dup_result = dup2(fd, STDOUT_FILENO);
	if (dup_result == -1)
	{
		close(fd);
		shell->env->exit_status = 1;
		return (0);
	}
	close(fd);
	return (1);
}

int	process_redirs(t_shell *shell, t_token **last_out)
{
	t_token	*curr;

	curr = shell->tk;
	while (curr)
	{
		if ((curr->type == T_GREAT || curr->type == T_DGREAT) && curr->next)
		{
			if (!open_output_file(curr, shell))
				return (0);
			*last_out = curr;
		}
		else if (curr->type == T_LESS && curr->next)
		{
			if (!open_file_input(curr, shell))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}

void	exec_filtered_cmd(t_shell *shell, t_token *cmd_tokens)
{
	shell->tk = cmd_tokens;
	remove_added_quotes(&cmd_tokens->cmd);
	if (ft_is_builtin(cmd_tokens->cmd))
		handle_builtin(shell, cmd_tokens->cmd);
	else
		handle_path_command(shell, cmd_tokens->cmd);
	free_token_list(cmd_tokens);
}

int	handle_redirection(t_token *tk, t_shell *shell)
{
	t_token	*curr;
	t_token	*last_out;
	t_token	*cmd_tokens;

	last_out = NULL;
	if (check_redirect_syntax(shell))
		return (2);
	if (!process_redirs(shell, &last_out))
		return (1);
	if (last_out && !redirect_last_out(last_out, shell))
		return (1);
	curr = shell->tk;
	while (curr)
	{
		if (curr->type == T_DLESS || curr->type == T_LESS)
			return (handle_input_redirect(curr, shell));
		curr = curr->next;
	}
	cmd_tokens = filter_cmd_tokens(tk);
	if (cmd_tokens && cmd_tokens->cmd)
		exec_filtered_cmd(shell, cmd_tokens);
	return (1);
}
