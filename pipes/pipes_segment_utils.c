/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_segment_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by njoudieh          #+#    #+#             */
/*   Updated: 2025/08/16 10:19:52 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

static int	is_identifier_command(t_token *curr)
{
	if (curr->type != T_IDENTIFIER)
		return (0);
	if (!curr->prev)
		return (1);
	if (curr->prev->type != T_GREAT && curr->prev->type != T_DGREAT
		&& curr->prev->type != T_LESS && curr->prev->type != T_DLESS)
		return (1);
	return (0);
}

int	has_cmd_before_heredoc(t_token *tk)
{
	t_token	*curr;
	int		found_heredoc;
	int		found_command;

	curr = tk;
	found_heredoc = 0;
	found_command = 0;
	while (curr)
	{
		if (curr->type == T_PIPE)
			break ;
		if (curr->type == T_DLESS)
		{
			found_heredoc = 1;
			break ;
		}
		if (is_identifier_command(curr))
			found_command = 1;
		curr = curr->next;
	}
	return (found_heredoc && found_command);
}

static void	setup_redirections(t_pipe_child_data *child_data,
			int has_redirection, int has_actual_command)
{
	if (has_redirection)
	{
		if (has_actual_command)
		{
			setup_input_redirection(child_data);
			setup_output_redirection(child_data);
		}
		setup_input_redirection(child_data);
		setup_output_redirection(child_data);
		child_data->cmd_segment = remove_redirection_tokens(
				child_data->cmd_segment);
	}
}

static void	execute_command(t_pipe_child_data *child_data, t_shell *shell)
{
	if (ft_pipe_builtin(child_data->cmd_segment))
	{
		shell->tk = child_data->cmd_segment;
		handle_builtin(shell, child_data->cmd_segment->cmd);
		free_token_list(child_data->cmd_segment);
		exit(EXIT_SUCCESS);
	}
	execute_external_cmd(child_data->cmd_segment, child_data->ft_env, shell);
}

void	handle_redirections_and_execute(t_pipe_child_data *child_data,
		t_shell *shell)
{
	int	has_redirection;
	int	has_actual_command;

	has_redirection = (contain_list(">>", child_data->cmd_segment)
			|| contain_list(">", child_data->cmd_segment)
			|| contain_list("<", child_data->cmd_segment));
	has_actual_command = has_cmd_before_heredoc(child_data->cmd_segment);
	setup_redirections(child_data, has_redirection, has_actual_command);
	if (!child_data->cmd_segment || !child_data->cmd_segment->cmd)
	{
		free_token_list(child_data->cmd_segment);
		exit(0);
	}
	execute_command(child_data, shell);
}
