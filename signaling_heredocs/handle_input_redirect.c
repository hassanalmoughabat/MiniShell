/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:00:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 21:30:05 by njoudieh42       ###   ########.fr       */
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

int	handle_less(char *filename, t_shell *shell)
{
	int				fd;
	int				result;
	t_token *curr = shell->tk;
	t_token *last_in = NULL;

	result = handle_standalone_less(filename, shell);
	if (result != -1)
		return (result);
	while (curr)
	{
		if (curr->type == T_LESS && curr->next && is_valid_filename(curr->next))
		{
			last_in = curr;
			remove_added_quotes(&last_in->next->cmd);
			fd = open(last_in->next->cmd, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(last_in->next->cmd, 2);
				ft_putstr_fd(": ", 2);
				ft_putendl_fd(strerror(errno), 2);
				shell->env->exit_status = 1;
				return (1);
			}
		}
		curr = curr->next;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
			ft_putstr_fd("minishell: dup2: ", 2);
			ft_putendl_fd(strerror(errno), 2);
			close(fd);
			shell->env->exit_status = 1;
			return (1);
		}
		close(fd);
	t_token *curr2 = shell->tk;
	t_token *cmd_tokens = NULL;
	t_token *last_cmd = NULL;
	while (curr2)
	{
		if (curr2->type == T_GREAT || curr2->type == T_DGREAT
			|| curr2->type == T_LESS || curr2->type == T_DLESS)
		{
			curr2 = curr2->next;
			if (curr2)
				curr2 = curr2->next;
			continue ;
		}
		t_token *new_token = malloc(sizeof(t_token));
		new_token->cmd = ft_strdup(curr2->cmd);
		new_token->type = curr2->type;
		new_token->next = NULL;
		new_token->prev = NULL;
		if (!cmd_tokens)
			cmd_tokens = new_token;
		else
			last_cmd->next = new_token;
		last_cmd = new_token;
		curr2 = curr2->next;
	}
	if (cmd_tokens && cmd_tokens->cmd)
	{
		shell->tk = cmd_tokens;
		remove_added_quotes(&cmd_tokens->cmd);
		if (ft_is_builtin(cmd_tokens->cmd))
			handle_builtin(shell, cmd_tokens->cmd);
		else
			handle_path_command(shell, cmd_tokens->cmd);
		free_token_list(cmd_tokens);
	}
	return (1);
}
