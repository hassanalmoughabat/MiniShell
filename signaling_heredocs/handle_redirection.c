/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/08 18:26:28 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	contain_list(char *str, t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if (ft_strcmp(curr->cmd, str) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	is_valid_filename(t_token *token)
{
	if (!token || !token->cmd)
		return (0);
	// if (token->type != T_IDENTIFIER)
	// 	return (0);
	// if (ft_strcmp(token->cmd, ">") == 0 || ft_strcmp(token->cmd, ">>") == 0 ||
	// 	ft_strcmp(token->cmd, "<") == 0 || ft_strcmp(token->cmd, "<<") == 0)
	// 	return (0);
	if (ft_isalnum(token->cmd[0]) || token->cmd[0] == '.'
		|| token->cmd[0] == '_' || token->cmd[0] == '/')
		return (1);
	return (1);
}

int	handle_output_redirect(t_token *curr, t_token *tk,
		char **ft_env)
{
	if (curr->next && is_valid_filename(curr->next))
	{
		g_minishell.env->exit_status = 0;
		if (curr->type == T_DGREAT)
			return (handle_dgreat(curr->next->cmd, tk,
					ft_env, g_minishell.env));
		else
			return (handle_great(curr->next->cmd, tk,
					ft_env, g_minishell.env));
	}
	return (0);
}

int	handle_input_redirect(t_token *curr, t_token *tk,
		char **ft_env, t_env *env)
{
	display_list(tk);
	if (curr->next && is_valid_filename(curr->next))
	{
		if (curr->type == T_DLESS)
		{
			g_minishell.signint_child = true;
			handle_heredoc(ft_env, env, tk);
		}
		else
			return (handle_less(curr->next->cmd, tk, ft_env, env));
	}
	return (0);
}

int	check_redirect_syntax(t_token *tk, char *input)
{
	t_token	*curr;

	if (!input)
		return (0);
	curr = tk;
	while (curr)
	{
		if (curr->type == T_GREAT || curr->type == T_DGREAT
			|| curr->type == T_LESS || curr->type == T_DLESS)
		{
			if (!curr->next || !is_valid_filename(curr->next))
			{
				ft_putstr_fd("minishell: syntax error", 2);
				ft_putendl_fd(" near unexpected token `newline'", 2);
				g_minishell.env->exit_status = 2;
				return (2);
			}
			else if (curr->next && curr->next->type != T_IDENTIFIER)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				if (curr->next && curr->next->type != T_IDENTIFIER)
					ft_putendl_fd(curr->next->cmd, 2);
				else
					ft_putendl_fd("newline", 2);
				return (g_minishell.env->exit_status = 2, 2);
			}
		}
		curr = curr->next;
	}
	return (0);
}

int handle_redirection(t_token *tk, char **ft_env, t_env *env, char *input)
{
	t_token *curr;
	t_token *last_out = NULL;
	int fd = -1;
	int syntax_check = check_redirect_syntax(tk, input);
	if (syntax_check)
		return (syntax_check);
	curr = tk;
	while (curr)
	{
		if ((curr->type == T_GREAT || curr->type == T_DGREAT) && curr->next && is_valid_filename(curr->next))
		{
			int flags = (curr->type == T_GREAT) ? (O_WRONLY | O_CREAT | O_TRUNC) : (O_WRONLY | O_CREAT | O_APPEND);
			int tmp_fd = open(curr->next->cmd, flags, 0644);
			if (tmp_fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(curr->next->cmd, 2);
				ft_putstr_fd(": ", 2);
				ft_putendl_fd(strerror(errno), 2);
				g_minishell.env->exit_status = 1;
				return (1);
			}
			close(tmp_fd);
			last_out = curr;
		}
		else if (curr->type == T_LESS && curr->next && is_valid_filename(curr->next))
		{
			int test_fd = open(curr->next->cmd, O_RDONLY);
			if (test_fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(curr->next->cmd, 2);
				ft_putstr_fd(": ", 2);
				ft_putendl_fd(strerror(errno), 2);
				g_minishell.env->exit_status = 1;
				return (1);
			}
			close(test_fd);
		}
		curr = curr->next;
	}
	if (last_out)
	{
		int flags = (last_out->type == T_GREAT) ? (O_WRONLY | O_CREAT | O_TRUNC) : (O_WRONLY | O_CREAT | O_APPEND);
		fd = open(last_out->next->cmd, flags, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(last_out->next->cmd, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			g_minishell.env->exit_status = 1;
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			g_minishell.env->exit_status = 1;
			return (1);
		}
		close(fd);
	}
	curr = tk;
	while (curr)
	{
		if (curr->type == T_DLESS || curr->type == T_LESS)
			return (handle_input_redirect(curr, tk, ft_env, env));
		curr = curr->next;
	}
	t_token *curr2 = tk;
	t_token *cmd_tokens = NULL;
	t_token *last_cmd = NULL;
	while (curr2) 
	{
		if (curr2->type == T_GREAT || curr2->type == T_DGREAT ||
			curr2->type == T_LESS || curr2->type == T_DLESS)
			{
				curr2 = curr2->next;
				if (curr2) curr2 = curr2->next;
				continue;
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
if (cmd_tokens && cmd_tokens->cmd) {
    if (ft_is_builtin(cmd_tokens->cmd))
        handle_builtin(cmd_tokens, ft_env, &env);
    else
        handle_path_command(cmd_tokens, ft_env, cmd_tokens->cmd);
    free_token_list(cmd_tokens);
}
	return (1);
}
