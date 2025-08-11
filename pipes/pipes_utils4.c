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

void	setup_pipe_data(t_pipe_data *data, t_token *lst, t_shell *shell)
{
	int				pipe_count;
	t_heredoc_info	*heredocs;
	int				hd_count;

	pipe_count = count_pipes(lst);
	heredocs = process_heredocs_before_pipes(lst, shell, &hd_count);
	data->lst = lst;
	data->ft_env = shell->ft_env;
	data->env = shell->env;
	data->pipe_count = pipe_count;
	data->heredocs = heredocs;
	data->hd_count = hd_count;
	data->last_child_pid = -1;
}

void	handle_pipe(t_token *lst, t_shell *shell, char *input)
{
	int			**pipes;
	t_pipe_data	data;
	int			i;

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
		if (data.heredocs)
		{
			i = -1;
			while (++i < data.hd_count)
				close(data.heredocs[i].fd);
			free(data.heredocs);
		}
		return ((void)(shell->env->exit_status = 1));
	}
	data.pipes = pipes;
	execute_pipe_commands(&data, shell);
}

static void	print_error(char *str)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\'\n", 2);
}

int	valid_pipe(t_shell *shell, char *input)
{
	t_token	*tk;

	tk = shell->tk;
	while (tk)
	{
		if (tk->type == T_PIPE)
		{
			if ((tk->next->type == T_PIPE))
			{
				ft_putstr_fd("bash: syntax error near unexpected token `", 2);
				pipe_error_message(input, tk);
				ft_putstr_fd("\'\n", 2);
				return (shell->env->exit_status = 2, 0);
			}
			if (tk->next->type == T_IDENTIFIER
				&& tk->prev->type != T_IDENTIFIER)
			{
				print_error(tk->cmd);
				return (shell->env->exit_status = 2, 0);
			}
		}
		tk = tk->next;
	}
	return (1);
}
