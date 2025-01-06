#include "includes/ft_printf/ft_printf.h"
#include "includes/minishell.h"


char *path_extract(char *str, int count)
{
    int i = count;
    while(str[i])
        i++;
    int word_len = i - count;
    char *path;
    path = malloc(sizeof(char) * (word_len + 1));
    if (!path)
        return (0);
    int j = 0;
    while (str[count])
    {
        path[j] = str[count];
        j++;
        count++;
    }
    path[j] = '\0';
    return path;
}

char *get_my_path(t_env *env)
{
    t_env *current;
    char *path;

    if (!env)
    {
        ft_printf("DEBUG: env is NULL\n");
        return NULL;
    }

    current = env;
    while (current)
    {
        ft_printf("DEBUG: Checking env line: %s\n", current->line);
        if (ft_strncmp(current->line, "PATH=", 5) == 0)
        {
            path = path_extract(current->line, 5);
            return (path);
        }
        current = current->next;
    }
    ft_printf("DEBUG: PATH not found in environment\n");
    return (NULL);
}

char **transform(t_env *env)
{
    char **ftenvp;
    int count = 0;
    t_env *current = env;

    while (current)
    {
        count++;
        current = current->next;
    }

    ftenvp = (char **)malloc(sizeof(char *) * (count + 1));
    if (!ftenvp)
        return NULL;

    current = env;
    int i = 0;
    while (current && i < count)
    {
        ftenvp[i] = ft_strdup(current->line);
        i++;
        current = current->next;
    }
    ftenvp[i] = NULL;  // NULL terminate the array

    return ftenvp;
}


// Return copy of original command if not found
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

    // if (is_builtin(s_cmd[0]))
    // {
    //     handle_builtin(s_cmd, envp);
    //     ft_free_tab(s_cmd);
    //     return;
    // }

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



// int ft_is_builtin(char *cmd)
// {
//     return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
//             !strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
//             !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
//             !strcmp(cmd, "exit"));
// }



// void exec_command(char *cmd, char *envp)
// {

//     return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") ||
//             !strcmp(cmd, "pwd") || !strcmp(cmd, "export") ||
//             !strcmp(cmd, "unset") || !strcmp(cmd, "env") ||
//             !strcmp(cmd, "exit"));
// }



