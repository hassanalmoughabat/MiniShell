#include "includes/minihell.h"


// int contain_more_than_command(t_token *tk, char **ft_env)
// {
// 	t_token *curr;

// 	curr = tk;
// 	while (curr)
// 	{
		
// 	}
// }
void	handle_pipe(t_token *lst, char **ft_env, t_env * env)
{

}

void handle_redirection(t_token *tk, char **ft_env, t_env *env)
{

}






int	is_pipe(char *ft_value)
{
	int i = 0;

	while (ft_value)
	{
		if (ft_value[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

int is_redirection(char *value)
{
	int i = 0;

	while (value[i])
	{
		if (value[i] == '<' || value[i] == '>')
			return (1);
		else if((value[i] && value[i + 1]) && value[i] == '<' && value[i + 1] == '<')
			return (1);
		else if ((value[i] && value[i + 1]) && value[i] == '>' && value[i + 1] == '>')
			return (1);
		i++;
	}
	return (0);
}

int ft_is_builtin(char *cmd)
{
	 char **builtins = 
	 {
		"cd",
		"env",
		"echo",
		"export",
		"unset",
		"env",
		"exit",
		NULL,
	 };
	 int	i;

	 i = 0;
	 while (*builtins[i])
	 {
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
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
		if (strncmp(ft_env[i], "PATH=", 5) == 0)
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
		{
			ft_printf("Error: Memory allocation failed in ft_substr\n");
			return (0);
		}
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
		{
			ft_printf("Error: Memory allocation failed in ft_strjoin\n");
			return (0);
		}
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


void handle_path_command(char *envp[], char *cmd)
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

void	after_parsing(t_token *tk, char **ft_env, t_env *env, char *input)
{
	ft_printf("in after parsing\n");
	t_token *curr;
	int command = 0;

	curr = tk;
	// while (curr)
	// {
	// 	if (is_pipe(curr->cmd))
	// 		handle_pipe(tk, ft_env, env);
	// 	else if (is_redirection(curr->cmd))
	// 		handle_redirection(tk, ft_env, env);
	// 	else if (ft_is_builtin(curr->cmd))
	// 		handle_builtin(tk, ft_env, env);
	// 	else 
	// 	{
	// 		ft_printf("before is a path command\n");
	// 			handle_path_command(ft_env, input);
	// 	}
	// 	curr = curr->next;
	// }
	ft_printf("before is a path command\n");
				handle_path_command(ft_env, input);
}