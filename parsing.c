/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:42 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/10 20:36:45 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

void	handle_pipe(t_token *lst, char **ft_env, t_env * env)
{
	ft_printf("hey");
}
static int	ft_exist(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_is_builtin(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if ((ft_strcmp(curr->cmd, "cd") == 0))
			return (1);
		else if (ft_strcmp(curr->cmd, "echo") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "unset") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "env") == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	handle_path_command(char *envp[], char *cmd)
{
	char	**s_cmd;
	char	*path;
	pid_t	pid;
	int		status;

	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd || !s_cmd[0])
		error_print_free("minishell: command not found", 1, s_cmd);
	pid = fork();
	if (pid == -1)
		error_print_free("fork", 1, s_cmd);
	else if (pid == 0)
	{
		path = get_path(s_cmd[0], envp);
		if (!path || execve(path, s_cmd, envp) == -1)
			error_print_free("minishell: command not found", 1, s_cmd);
	}
	else
	{
		waitpid(pid, &status, 0);
		ft_free_tab(s_cmd);
	}
}

void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input)
{
	t_token	*curr;
	int		command;
	int flag;
	
	flag = 0;
	command = 0;
	curr = tk;
	
	if (curr->type == T_PIPE)
	{
		handle_pipe(tk, ft_env, *env);
	}
	else if (contain_list("<<", tk) || contain_list(">>", tk)
			|| contain_list("<", tk) || contain_list(">", tk))
	{
		handle_redirection(tk, ft_env, *env);
	}
	else if (ft_is_builtin(curr))
	{
		handle_builtin(tk, ft_env, *env, input);
	}
	else
		handle_path_command(ft_env, input);
	return ;
}
