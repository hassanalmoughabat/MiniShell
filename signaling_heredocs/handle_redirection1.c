/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 10:37:43 by hal-moug         ###   ########.fr       */
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
	if (token->cmd[0] == '\0')
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
			handle_heredoc(shell);
		else
			return (handle_less(curr->next->cmd, shell));
	}
	return (0);
}

void	syntax_error_msg(t_token *curr)
{
	if (!curr->next)
	{
		ft_putstr_fd("minishell: syntax error", 2);
		ft_putendl_fd(" near unexpected token `newline'", 2);
	}
	else if (curr->next->type == T_GREAT || curr->next->type == T_DGREAT
		|| curr->next->type == T_LESS || curr->next->type == T_DLESS
		|| curr->next->type == T_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(curr->next->cmd, 2);
		ft_putendl_fd("'", 2);
	}
	else if (!is_valid_filename(curr->next))
	{
		ft_putstr_fd("minishell: syntax error", 2);
		ft_putendl_fd(" near unexpected token `newline'", 2);
	}
	return ;
}


int	check_triple_redirect_in_tokens(t_shell *shell)
{
	t_token	*curr;

	curr = shell->tk;
	while (curr)
	{
		if (!ft_strncmp(curr->cmd, ">>>>", 4)
			|| !ft_strncmp(curr->cmd, "<<<<", 4)
			|| !ft_strncmp(curr->cmd, ">>>", 3))
		{
			shell->env->exit_status = 2;
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			if (curr->cmd[0] == '>')
				ft_putendl_fd(">'", 2);
			else
				ft_putendl_fd("<'", 2);
			return (2);
		}
		curr = curr->next;
	}
	return (0);
}

int	check_redirect_syntax(t_shell *shell)
{
	t_token	*curr;
	
	if (check_triple_redirect_in_tokens(shell))
		return (-1);
	curr = shell->tk;
	while (curr)
	{
		if (curr->type == T_GREAT || curr->type == T_DGREAT
			|| curr->type == T_LESS || curr->type == T_DLESS)
		{
			if (!curr->next)
			{
				syntax_error_msg(curr);
				return (shell->env->exit_status = 2, 2);
			}
			if (curr->next->type == T_GREAT || curr->next->type == T_DGREAT
				|| curr->next->type == T_LESS || curr->next->type == T_DLESS
				|| curr->next->type == T_PIPE)
			{
				syntax_error_msg(curr);
				return (shell->env->exit_status = 2, 2);
			}
		}
		curr = curr->next;
	}
	return (0);
}
