/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:42 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/27 20:16:35 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

void	handle_pipe(t_token *lst, char **ft_env, t_env * env)
{
	ft_printf("hey");
}
void handle_redirection(t_token *tk, char **ft_env, t_env *env)
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
		if ((ft_strcmp(curr->cmd, "cd") == 0)
			|| (ft_strcmp(curr->cmd, "env") == 0))
			return (1);
		else if ((ft_strcmp(curr->cmd, "env") == 0)
			|| (ft_strcmp(curr->cmd, "echo") == 0))
			return (1);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "unset") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	is_a_path_command(char *cmd, char **ft_env)
{
	char	*path_env;
	int		i;
	char	*start;
	char	*colon;
	char	*dir;
	char	*tmp;
	char	*full_path;
	int		found;
	size_t	len;

	path_env = NULL;
	i = 0;
	while (ft_env[i])
	{
		if (ft_strncmp(ft_env[i], "PATH=", 5) == 0)
		{
			path_env = ft_env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
	{
		ft_printf("Error: PATH environment variable not found\n");
		return (0);
	}
	found = 0;
	start = path_env;
	while (*start)
	{
		colon = ft_strchr(start, ':');
		if (colon)
			len = (size_t)(colon - start);
		else
			len = ft_strlen(start);
		dir = ft_substr(start, 0, len);
		if (!dir)
			return (0);
		tmp = ft_strjoin(dir, "/");
		free(dir);
		if (!tmp)
		{
			ft_printf("Error: Memory allocation failed in ft_strjoin\n");
			return (0);
		}
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			found = 1;
			free(full_path);
			break ;
		}
		free(full_path);
		if (colon)
			start = colon + 1;
		else
			break ;
	}
	if (!found)
	{
		ft_printf("Error: Command Not Found\n");
		return (0);
	}
	return (1);
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

	command = 0;
	curr = tk;
	if (curr->type == T_PIPE)
		handle_pipe(tk, ft_env, *env);
	else if ((curr->type == T_DGREAT || curr->type == T_DLESS
			|| curr->type == T_GREAT || curr->type == T_LESS))
		handle_redirection(tk, ft_env, *env);
	else if (ft_is_builtin(curr))
		handle_builtin(tk, ft_env, *env, input);
	else
		handle_path_command(ft_env, input);
	return ;
}
