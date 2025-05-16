/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/04/11 10:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

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

 int	handle_output_redirect(t_token *curr, t_token *tk,
		char **ft_env, t_env *env)
{
	if (curr->next && curr->next->type == T_IDENTIFIER)
	{
		if (curr->type == T_DGREAT)
			return (handle_dgreat(curr->next->cmd, tk, ft_env, env));
		else
			return (handle_great(curr->next->cmd, tk, ft_env, env));
	}
	else
	{
		ft_putstr_fd("minishell: syntax error", 2);
		ft_putendl_fd(" near unexpected token `newline'", 2);
		return (0);
	}
}

 int	handle_input_redirect(t_token *curr, t_token *tk,
		char **ft_env, t_env *env)
{
	if (curr->next && curr->next->type == T_IDENTIFIER)
	{
			handle_cat_heredoc(ft_env, env, tk);
		return (1);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error", 2);
		ft_putendl_fd(" near unexpected token `newline'", 2);
		return (0);
	}
}

int	handle_redirection(t_token *tk, char **ft_env, t_env *env)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if (curr->type == T_DGREAT || curr->type == T_GREAT)
			return (handle_output_redirect(curr, tk, ft_env, env));
		else if (curr->type == T_DLESS || curr->type == T_LESS)
			return (handle_input_redirect(curr, tk, ft_env, env));
		curr = curr->next;
	}
	return (1);
}
