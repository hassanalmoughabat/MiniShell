/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 15:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	is_redirect_token(t_token *token)
{
	return (token->type == T_GREAT || token->type == T_DGREAT
		|| token->type == T_LESS || token->type == T_DLESS);
}

int	is_invalid_next_token(t_token *next)
{
	return (next->type == T_GREAT || next->type == T_DGREAT
		|| next->type == T_LESS || next->type == T_DLESS
		|| next->type == T_PIPE);
}

int	check_redirect_error(t_token *curr, t_shell *shell)
{
	if (!curr->next)
	{
		syntax_error_msg(curr);
		return (shell->env->exit_status = 2, 2);
	}
	if (is_invalid_next_token(curr->next))
	{
		syntax_error_msg(curr);
		return (shell->env->exit_status = 2, 2);
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
}
