/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/27 00:55:53 by njoudieh42       ###   ########.fr       */
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
	if (ft_isalnum(token->cmd[0]) || token->cmd[0] == '.'
		|| token->cmd[0] == '_' || token->cmd[0] == '/')
		return (1);
	return (1);
}

int	handle_input_redirect(t_token *curr, t_shell *shell)
{
	if (curr->next && is_valid_filename(curr->next))
	{
		if (curr->type == T_DLESS)
		{
			g_signal.signint_child = true;
			g_signal.heredoc_sigint = false;
			handle_heredoc(shell);
		}
		else
			return (handle_less(curr->next->cmd, shell));
	}
	return (0);
}

void	syntax_error_msg(t_token *curr)
{
	if (!curr->next || !is_valid_filename(curr->next))
	{
		ft_putstr_fd("minishell: syntax error", 2);
		ft_putendl_fd(" near unexpected token `newline'", 2);
	}
	else if (curr->next && curr->next->type != T_IDENTIFIER)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (curr->next && curr->next->type != T_IDENTIFIER)
			ft_putendl_fd(curr->next->cmd, 2);
		else
			ft_putendl_fd("newline", 2);
	}
	return ;
}

int	check_redirect_syntax(t_shell *shell, char *input)
{
	t_token	*curr;

	if (!input)
		return (0);
	curr = shell->tk;
	while (curr)
	{
		if (curr->type == T_GREAT || curr->type == T_DGREAT
			|| curr->type == T_LESS || curr->type == T_DLESS)
		{
			if (!curr->next || !is_valid_filename(curr->next))
			{
				syntax_error_msg(curr);
				return (shell->env->exit_status = 2, 2);
			}
		}
		curr = curr->next;
	}
	return (0);
}
