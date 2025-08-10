/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/26 17:26:34 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	pipe_error_message(char *input, t_token *tk)
{
	if (!tk->prev && tk->next)
	{
		if (tk->next->type == T_PIPE)
		{
			if (ft_check_space(input[ft_index(input, '|') + 1]))
				ft_putstr_fd(tk->cmd, 2);
			else
				ft_putstr_fd("||", 2);
		}
		else if (tk->next->type == T_IDENTIFIER)
			ft_putstr_fd(tk->cmd, 2);
	}
	else if (tk->next && tk->prev)
	{
		if (tk->next->type != T_IDENTIFIER && tk->prev->type == T_IDENTIFIER)
		{
			if (tk->next->type == T_PIPE)
				ft_putstr_fd(tk->cmd, 2);
			else
				ft_putstr_fd("newline", 2);
		}
		else if (tk->prev->type != T_IDENTIFIER
			&& tk->next->type != T_IDENTIFIER)
			ft_putstr_fd(tk->cmd, 2);
	}
}

static int	check_pipe_next_token(t_token *tk, t_shell *shell, char *input)
{
	if (!tk->next)
		return (pipe_syntax_error(tk->cmd, shell), 0);
	if (tk->next->type == T_PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		pipe_error_message(input, tk);
		return (ft_putstr_fd("\'\n", 2), shell->env->exit_status = 2, 0);
	}
	return (1);
}

static int	check_pipe_prev_token(t_token *tk, t_shell *shell)
{
	if (!tk->prev)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(tk->cmd, 2);
		ft_putstr_fd("\'\n", 2);
		return (shell->env->exit_status = 2, 0);
	}
	if (tk->next->type == T_IDENTIFIER && tk->prev->type != T_IDENTIFIER)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(tk->cmd, 2);
		ft_putstr_fd("\'\n", 2);
		return (shell->env->exit_status = 2, 0);
	}
	return (1);
}

int	valid_pipe(t_shell *shell, char *input)
{
	t_token	*tk;

	tk = shell->tk;
	while (tk)
	{
		if (tk->type == T_PIPE)
		{
			if (!check_pipe_next_token(tk, shell, input))
				return (0);
			if (!check_pipe_prev_token(tk, shell))
				return (0);
		}
		tk = tk->next;
	}
	return (1);
}

static void	handle_cleanup_heredocs(t_pipe_data *data)
{
	int	i;

	if (data->heredocs)
	{
		i = -1;
		while (++i < data->hd_count)
			close(data->heredocs[i].fd);
		free(data->heredocs);
	}
}

void	handle_pipe(t_token *lst, t_shell *shell, char *input)
{
	int			**pipes;
	t_pipe_data	data;

	if (!lst || !valid_pipe(shell, input))
		return ;
	if (has_heredoc(lst) && count_pipes(lst) == 1)
	{
		if (check_special_heredoc_pipe(lst, shell))
			return ;
	}
	setup_pipe_data(&data, lst, shell);
	if (create_pipes(&pipes, data.pipe_count) == -1)
	{
		handle_cleanup_heredocs(&data);
		return ;
	}
	data.pipes = pipes;
	execute_pipe_commands(&data, shell);
}
