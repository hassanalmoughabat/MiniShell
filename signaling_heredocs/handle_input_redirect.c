/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:00:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/28 13:23:01 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	open_input_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	return (fd);
}

int	handle_standalone_less(char *filename, t_token *tk)
{
	int	fd;

	if (tk && tk->cmd && ft_strcmp(tk->cmd, "<") == 0)
	{
		fd = open_input_file(filename);
		if (fd == -1)
			return (0);
		close(fd);
		return (1);
	}
	return (-1);
}

void	execute_with_input_redirect(t_token *cmd_tokens, char **ft_env,
		t_env *env, int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	if (ft_is_builtin(cmd_tokens->cmd))
		execute_builtin_redirect(cmd_tokens, ft_env, env);
	else
		execute_external_cmd(cmd_tokens, ft_env);
}

int	handle_exit_special(t_token *cmd_tokens, t_env *env, t_env **copy)
{
	t_token	*curr;

	if (!cmd_tokens || !cmd_tokens->cmd
		|| ft_strcmp(cmd_tokens->cmd, "exit") != 0)
		return (0);
	ft_putstr_fd("exit\n", 2);
	curr = cmd_tokens;
	if (!curr->next)
		exit(env->exit_status);
	decrement(&g_minishell);
	update_shlvl_in_env(&g_minishell.env, g_minishell.shell_level);
	if (curr->next)
		handle_exit_code(curr->next);
	else
		env->exit_status = 0;
	free_env_list(*copy);
	exit(env->exit_status);
	return (1);
}

int	handle_less_child(int fd, t_token *tk, t_token *redirect_token,
		t_redir *params)
{
	t_token	*cmd_tokens;

	cmd_tokens = copy_tokens_before_redirect(tk, redirect_token);
	if (!cmd_tokens)
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	execute_with_input_redirect(cmd_tokens, params->ft_env, params->env, fd);
	return (0);
}

int	handle_less_fork(int fd, t_token *tk, t_token *redirect_token,
		t_redir *params)
{
	pid_t	pid;
	int		status;
	t_token	*cmd_tokens;

	cmd_tokens = copy_tokens_before_redirect(tk, redirect_token);
	if (cmd_tokens && cmd_tokens->cmd && ft_strcmp(cmd_tokens->cmd,
			"exit") == 0)
	{
		int	stdin_backup;

		stdin_backup = dup(STDIN_FILENO);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			ft_putstr_fd("minishell: dup2: ", 2);
			ft_putendl_fd(strerror(errno), 2);
			close(fd);
			return (0);
		}
		close(fd);
		handle_exit_special(cmd_tokens, params->env, &params->env);
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		return (0);
	}
	else if (pid == 0)
		handle_less_child(fd, tk, redirect_token, params);
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			params->env->exit_status = 128 + WTERMSIG(status);
		else
			params->env->exit_status = WEXITSTATUS(status);
	}
	return (1);
}

int	handle_less(char *filename, t_token *tk, char **ft_env, t_env *env)
{
	int				fd;
	int				result;
	t_token			*redirect_token;
	t_redir			*params;

	result = handle_standalone_less(filename, tk);
	if (result != -1)
		return (result);
	redirect_token = find_redirect_token(tk, "<");
	fd = open_input_file(filename);
	if (fd == -1)
		return (0);
	if (!redirect_token || tk == redirect_token)
	{
		close(fd);
		return (1);
	}
	params = init_redir_params(ft_env, env);
	if (!params)
		return (close(fd), 0);
	result = handle_less_fork(fd, tk, redirect_token, params);
	free(params);
	return (result);
}
