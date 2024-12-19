#include "includes/minihell.h"

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	if (!s1)
		return (1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    if (strcmp(cmd, "cd") == 0)
        return 1;
    return 0;
}

int handle_builtin(char **args, char **envp)
{
	char *home;

    (void)envp; 
    if (!args || !args[0])
        return 0;

    if (strcmp(args[0], "cd") == 0)
    {
        if (!args[1])
        {
            home = my_getenv("HOME", envp);
            if (home && chdir(home) != 0)
                perror("cd");
        }
        else if (chdir(args[1]) != 0)
        {
            perror("cd");
        }
        return 1;
    }
    return 0;
}

void exec(char *cmd, char *envp[])
{
    char **s_cmd;
    char *path;
    pid_t pid;
    int status;

    s_cmd = ft_split(cmd, ' ');
    if (!s_cmd || !s_cmd[0])
    {
        ft_putstr_fd("minihell: command not found: ", 2);
        ft_putendl_fd(cmd, 2);
        ft_free_tab(s_cmd);
        return;
    }

    if (is_builtin(s_cmd[0]))
    {
        handle_builtin(s_cmd, envp);
        ft_free_tab(s_cmd);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        ft_free_tab(s_cmd);
        return;
    }
    else if (pid == 0)
    {
        // Child process
        path = get_path(s_cmd[0], envp);
        if (!path || execve(path, s_cmd, envp) == -1)
        {
            ft_putstr_fd("minihell: command not found: ", 2);
            ft_putendl_fd(s_cmd[0], 2);
            if (path)
                free(path);
            ft_free_tab(s_cmd);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process
        waitpid(pid, &status, 0);
        ft_free_tab(s_cmd);
    }
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
