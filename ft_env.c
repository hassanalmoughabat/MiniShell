#include "includes/minishell.h"
#include "includes/ft_printf/ft_printf.h"

char	*my_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char *envp[])
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	i = 0;
	allpath = ft_split(my_getenv("PATH", envp), ':');
	s_cmd = ft_split(cmd, ' ');
	while (allpath[i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_tab(allpath);
	ft_free_tab(s_cmd);
	return (cmd);
}

// int ft_is_builtin(char *cmd)
// {
//     return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
//             !strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
//             !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
//             !strcmp(cmd, "exit"));
// }

void	exec(char *cmd, char *envp[])
{
	char	**s_cmd;
	char *path;
	pid_t pid;
	int status;

	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd || !s_cmd[0])
		error_print_free("command not found", 1, s_cmd);
	// if (ft_is_builtin)
	// {
	// 	handle_builtin(s_cmd, envp);
	// 	ft_free_tab(s_cmd);
	// }
	pid = fork();
	if (pid == -1)
		error_print_free("fork", 1, s_cmd);
	else if (pid == 0)
	{
		path = get_path(s_cmd[0], envp);
		if (!path || execve(path, s_cmd, envp) == -1)
		{
			ft_printf("command not found: %s", s_cmd[0]);
			if (path)
				free(path);
			ft_free_tab(s_cmd);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		ft_free_tab(s_cmd);
	}
}





// void exec_command(char *cmd, char *envp)
// {

//     return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
//             !strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
//             !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
//             !strcmp(cmd, "exit"));
// }



