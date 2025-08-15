/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:44:56 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/15 00:25:43 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	pipe_syntax_error(char *msg, t_shell *shell)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("'\n", 2);
	shell->env->exit_status = 2;
}

int	check_invalid_pipe_sequences(char *input)
{
	int	i;
	int	pipe_count;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			pipe_count = 0;
			while (input[i] == '|')
			{
				pipe_count++;
				i++;
			}
			if (pipe_count > 2)
				return (1);
			if (pipe_count == 1 && ft_check_space(input[i])
				&& input[i + 1] == '|')
				return (1);
			continue ;
		}
		i++;
	}
	return (0);
}

int	handle_first_token_pipe(t_token *tk, char *input, t_shell *shell)
{
	if (!ft_strcmp(tk->cmd, "|"))
	{
		if (!ft_check_space(input[1]) && tk->next
			&& !ft_strcmp(tk->next->cmd, "|"))
			return (pipe_syntax_error("||", shell), 0);
		return (pipe_syntax_error(tk->cmd, shell), 0);
	}
	return (1);
}

static int	skip_whitespace_and_check_pipe(char *input, int start)
{
	int	i;

	i = start;
	while (input[i] && ft_check_space(input[i]))
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

static int	check_single_redirect_pipe(char *input, int *i, char redirect)
{
	if (input[*i] == redirect && input[*i + 1] != redirect)
	{
		(*i)++;
		if (skip_whitespace_and_check_pipe(input, *i))
			return (1);
	}
	return (0);
}

int	check_double_redirect_pipe(char *input, int *i, char redirect)
{
	if (input[*i] == redirect && input[*i + 1] == redirect)
	{
		*i += 2;
		if (skip_whitespace_and_check_pipe(input, *i))
			return (1);
	}
	return (0);
}

int	check_redirection_pipe_sequence(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (check_double_redirect_pipe(input, &i, '<'))
			return (1);
		else if (check_double_redirect_pipe(input, &i, '>'))
			return (1);
		else if (check_single_redirect_pipe(input, &i, '<'))
			return (1);
		else if (check_single_redirect_pipe(input, &i, '>'))
			return (1);
		else
			i++;
	}
	return (0);
}

int	validate_pipe_token(t_token *tk, t_shell *shell)
{
	if (tk->type != T_PIPE)
		return (1);
	if (!tk->next)
		return (pipe_syntax_error(tk->cmd, shell), 0);
	if (!tk->prev)
		return (pipe_syntax_error(tk->cmd, shell), 0);
	return (1);
}

int	valid_pipe(t_shell *shell, char *input)
{
	t_token	*tk;

	tk = shell->tk;
	if (!tk)
		return (1);
	if (!handle_first_token_pipe(tk, input, shell))
		return (0);
	if (check_invalid_pipe_sequences(input))
		return (pipe_syntax_error("|", shell), 0);
	if (check_redirection_pipe_sequence(input))
		return (pipe_syntax_error("|", shell), 0);
	if (check_redirect_syntax(shell))
		return (0);
	while (tk)
	{
		if (!validate_pipe_token(tk, shell))
			return (0);
		tk = tk->next;
	}
	return (1);
}
