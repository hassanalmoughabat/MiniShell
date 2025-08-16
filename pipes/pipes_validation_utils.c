/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_validation_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:30:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/16 15:30:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

int	skip_whitespace_and_check_pipe(char *input, int start)
{
	int	i;

	i = start;
	while (input[i] && ft_check_space(input[i]))
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

int	check_single_redirect_pipe(char *input, int *i, char redirect)
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
