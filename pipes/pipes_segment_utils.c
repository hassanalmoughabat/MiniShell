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

t_token	*remove_redirection_tokens(t_token *cmd_segment)
{
	t_token	*curr;
	t_token	*next;

	curr = cmd_segment;
	while (curr)
	{
		if ((curr->type == T_GREAT || curr->type == T_DGREAT
				|| curr->type == T_LESS) && curr->next)
		{
			next = curr->next->next;
			if (curr->prev)
				curr->prev->next = next;
			else
				cmd_segment = next;
			if (next)
				next->prev = curr->prev;
			curr = next;
			continue ;
		}
		curr = curr->next;
	}
	return (cmd_segment);
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
			break;
		if (curr->type == T_DLESS)
		{
			found_heredoc = 1;
			break;
		}
		if (curr->type == T_IDENTIFIER)
		{
			if (!curr->prev || (curr->prev->type != T_GREAT && 
				curr->prev->type != T_DGREAT && curr->prev->type != T_LESS &&
				curr->prev->type != T_DLESS))
				found_command = 1;
		}
		curr = curr->next;
	}
	return (found_heredoc && found_command);
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
	if (!child_data->cmd_segment || !child_data->cmd_segment->cmd)
		exit(0);
		// //////////////////fghjlfdk//////////////////
	if (ft_pipe_builtin(child_data->cmd_segment))
	{
		shell->tk = child_data->cmd_segment;
		handle_builtin(shell, child_data->cmd_segment->cmd);
		free_token_list(child_data->cmd_segment);
		exit(EXIT_SUCCESS);
	}
	execute_external_cmd(child_data->cmd_segment, child_data->ft_env, shell);
}

void	setup_input_redirection(t_pipe_child_data *child_data)
{
	t_token	*curr;
	int		fd;
	char	*filename;

	curr = child_data->cmd_segment;
	while (curr)
	{
		if (curr->type == T_LESS && curr->next)
		{
			filename = curr->next->cmd;
			fd = open(filename, O_RDONLY);
			if (fd >= 0)
				dup2(fd, STDIN_FILENO);
			return ;
		}
		curr = curr->next;
	}
}

void	setup_output_redirection(t_pipe_child_data *child_data)
{
	t_token	*curr;

	curr = child_data->cmd_segment;
	while (curr)
	{
		if (curr->type == T_GREAT && curr->next)
		{
			handle_great_redirect(curr);
			return ;
		}
		else if (curr->type == T_DGREAT && curr->next)
		{
			handle_dgreat_redirect(curr);
			return ;
		}
		curr = curr->next;
	}
}
