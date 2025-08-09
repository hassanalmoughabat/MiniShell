/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/09 15:22:12 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

void	setup_pipe_signals(void)
{
	g_signal.signint_child = true;
}

void	restore_pipe_signals(void)
{
	g_signal.signint_child = false;
}

static void	close_parent_pipes(t_pipe_data *data, int current_cmd)
{
	if (current_cmd > 0)
		close(data->pipes[current_cmd - 1][0]);
	if (current_cmd < data->pipe_count)
		close(data->pipes[current_cmd][1]);
}

void	execute_single_command(t_pipe_data *data, t_token *start,
			t_token *curr, int i, t_shell *shell)
{
	t_token					*cmd_segment;
	t_pipe_child_data		child_data;
	t_child_setup_params	setup_params;
	pid_t					pid;
	int						heredoc_fd;

	heredoc_fd = setup_heredoc_for_command(data, start, curr);
	cmd_segment = extract_command_segment(start, curr);
	if (!cmd_segment)
		return ;
	pid = fork();
	if (pid == 0)
	{
		ft_set_child_signals();
		close_unused_heredocs(data, heredoc_fd);
		setup_params.cmd_segment = cmd_segment;
		setup_params.i = i;
		setup_params.heredoc_fd = heredoc_fd;
		setup_child_data(&child_data, data, &setup_params);
		handle_pipe_child(&child_data, shell);
		free_token_list(cmd_segment);
		exit(EXIT_FAILURE);
	}
	data->last_child_pid = pid;
	close_parent_pipes(data, i);
	free_token_list(cmd_segment);
}

void	execute_pipe_commands(t_pipe_data *data, t_shell *shell)
{
	t_token	*curr;
	t_token	*start;
	int		i;

	setup_pipe_signals();
	start = data->lst;
	curr = next_pipe(start);
	i = 0;
	while (1)
	{
		execute_single_command(data, start, curr, i, shell);
		if (!curr)
			break ;
		start = curr->next;
		curr = next_pipe(start);
		i++;
	}
	cleanup_heredocs_after_exec(data);
	wait_for_children(data, shell);
	restore_pipe_signals();
	ft_restore_parent_signals();
}

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
	// ///////////////////////////////////////////////////////////////////////
	data->last_child_pid = -1;
}

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

int	valid_pipe(t_shell *shell, char *input)
{
	t_token	*tk;

	tk = shell->tk;
	while (tk)
	{
		if (tk->type == T_PIPE)
		{
			if (!tk->next)
				return (pipe_syntax_error(tk->cmd, shell), 0);
			if ((tk->next->type == T_PIPE || tk->next->type != T_IDENTIFIER))
			{
				ft_putstr_fd("bash: syntax error near unexpected token `", 2);
				pipe_error_message(input, tk);
				ft_putstr_fd("\'\n", 2);
				return (shell->env->exit_status = 2, 0);
			}
            if (!tk->prev)
                {
                     ft_putstr_fd("bash: syntax error near unexpected token `", 2);
                     ft_putstr_fd(tk->cmd, 2);
                     ft_putstr_fd("\'\n", 2);
                    shell->env->exit_status = 2;
                    return (0);
                }
                if (tk->next->type == T_IDENTIFIER && tk->prev->type != T_IDENTIFIER)
                {
                    ft_putstr_fd("bash: syntax error near unexpected token `", 2);
                     ft_putstr_fd(tk->cmd, 2);
                     ft_putstr_fd("\'\n", 2);
                    shell->env->exit_status = 2;
                    return (0);
                }
		}
		tk = tk->next;
	}
	return (1);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/26 17:07:55 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
#include "pipes.h"

static void	close_parent_pipes(t_pipe_data *data, int current_cmd)
{
	if (current_cmd > 0)
		close(data->pipes[current_cmd - 1][0]);
	if (current_cmd < data->pipe_count)
		close(data->pipes[current_cmd][1]);
}

void	execute_single_command(t_pipe_data *data, t_token *start,
			t_token *curr, int i, t_shell *shell)
{
	t_token					*cmd_segment;
	t_pipe_child_data		child_data;
	t_child_setup_params	setup_params;
	pid_t					pid;
	int						heredoc_fd;

	heredoc_fd = setup_heredoc_for_command(data, start, curr);
	cmd_segment = extract_command_segment(start, curr);
	if (!cmd_segment)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close_unused_heredocs(data, heredoc_fd);
		setup_params.cmd_segment = cmd_segment;
		setup_params.i = i;
		setup_params.heredoc_fd = heredoc_fd;
		setup_child_data(&child_data, data, &setup_params);
		handle_pipe_child(&child_data, shell);
		exit(EXIT_FAILURE);
	}
	close_parent_pipes(data, i);
	free_token_list(cmd_segment);
}

void	execute_pipe_commands(t_pipe_data *data, t_shell *shell)
{
	t_token	*curr;
	t_token	*start;
	int		i;

	start = data->lst;
	curr = next_pipe(start);
	i = 0;
	while (1)
	{
		execute_single_command(data, start, curr, i, shell);
		if (!curr)
			break ;
		start = curr->next;
		curr = next_pipe(start);
		i++;
	}
	cleanup_heredocs_after_exec(data);
	wait_for_children(data);
}

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
}

void	pipe_syntax_error(char *msg, t_shell *shell)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("'\n", 2);
	shell->env->exit_status = 2;
}
