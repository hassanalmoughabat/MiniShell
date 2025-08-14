/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/09 01:29:15 by njoudieh42       ###   ########.fr       */
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

void	close_parent_heredoc(t_pipe_data *data, int heredoc_fd)
{
	int	j;

	if (heredoc_fd < 0)
		return ;
	j = 0;
	while (j < data->hd_count)
	{
		if (data->heredocs[j].fd == heredoc_fd)
		{
			close(data->heredocs[j].fd);
			data->heredocs[j].fd = -1;
			break ;
		}
		j++;
	}
}
