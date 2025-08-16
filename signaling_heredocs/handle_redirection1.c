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
	int		error;

	if (check_triple_redirect_in_tokens(shell))
		return (-1);
	curr = shell->tk;
	while (curr)
	{
		if (is_redirect_token(curr))
		{
			error = check_redirect_error(curr, shell);
			if (error)
				return (error);
		}
		curr = curr->next;
	}
	return (0);
}
