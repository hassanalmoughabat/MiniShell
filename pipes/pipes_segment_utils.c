/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by njoudieh          #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by hal-moug         ###   ########.fr       */
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

void	handle_redirections_and_execute(t_pipe_child_data *child_data)
{
	int	has_redirection;

	has_redirection = (contain_list(">>", child_data->cmd_segment)
			|| contain_list(">", child_data->cmd_segment)
			|| contain_list("<", child_data->cmd_segment));
	if (has_redirection)
	{
		setup_input_redirection(child_data);
		setup_output_redirection(child_data);
		child_data->cmd_segment = remove_redirection_tokens(
				child_data->cmd_segment);
	}
	if (ft_pipe_builtin(child_data->cmd_segment))
	{
		handle_builtin(child_data->cmd_segment, child_data->ft_env,
			&child_data->env);
		free_token_list(child_data->cmd_segment);
		exit(EXIT_SUCCESS);
	}
	execute_external_cmd(child_data->cmd_segment, child_data->ft_env);
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
