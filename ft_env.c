#include "includes/minishell.h"
#include "includes/ft_printf/ft_printf.h"





// int is_builtin(char *cmd)
// {
//     if (!cmd)
//         return 0;
//     if (strcmp(cmd, "cd") == 0)
//         return 1;
//     return 0;
// }

// int handle_builtin(char **args, char **envp)
// {
// 	char *home;

//     (void)envp; 
//     if (!args || !args[0])
//         return 0;

//     if (strcmp(args[0], "cd") == 0)
//     {
//         if (!args[1])
//         {
//             home = my_getenv("HOME", envp);
//             if (home && chdir(home) != 0)
//                 perror("cd");
//         }
//         else if (chdir(args[1]) != 0)
//         {
//             perror("cd");
//         }
//         return 1;
//     }
//     return 0;
// }



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
// Return copy of original command if not found


// void exec(char *cmd, char *envp[])
// {
//     char **s_cmd;
//     char *path;
//     pid_t pid;
//     int status;

//     if (!cmd || !envp)
//         return;

//     s_cmd = ft_split(cmd, ' ');
//     if (!s_cmd || !s_cmd[0])
//     {
//         ft_putstr_fd("minishell: command not found: ", 2);
//         ft_putendl_fd(cmd, 2);
//         ft_free_tab(s_cmd);
//         return;
//     }

//     // Uncomment and implement builtin handling if needed
//     /*
//     if (is_builtin(s_cmd[0]))
//     {
//         handle_builtin(s_cmd, envp);
//         ft_free_tab(s_cmd);
//         return;
//     }
//     */

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         ft_free_tab(s_cmd);
//         return;
//     }
//     else if (pid == 0)
//     {
//         path = get_path(s_cmd[0], envp);
//         if (!path || execve(path, s_cmd, envp) == -1)
//         {
//             ft_putstr_fd("minishell: command not found: ", 2);
//             ft_putendl_fd(s_cmd[0], 2);
//             if (path)
//                 free(path);
//             ft_free_tab(s_cmd);
//             exit(EXIT_FAILURE);
//         }
//     }
//     else
//     {
//         waitpid(pid, &status, 0);
//         ft_free_tab(s_cmd);
//     }
// }


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



