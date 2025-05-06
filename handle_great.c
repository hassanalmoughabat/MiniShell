#include "includes/minihell.h"

// Improved handle_dgreat function that properly handles append redirection
int handle_dgreat(char *filename, t_token *tk, char **ft_env, t_env *env)
{
    int fd;
    pid_t pid;
    int status;
    t_token *cmd_token = NULL;
    t_token *redirect_token = NULL;
    
    // Find command and redirection tokens
    cmd_token = tk;
    while (cmd_token)
    {
        if (cmd_token->cmd && cmd_token->next && 
            cmd_token->next->cmd && ft_strcmp(cmd_token->next->cmd, ">>") == 0)
        {
            redirect_token = cmd_token->next;
            break;
        }
        cmd_token = cmd_token->next;
    }
    
    // Open file with correct permissions
    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(filename, 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd(strerror(errno), 2);
        return 0;
    }
    
    // Fork and execute command
    pid = fork();
    if (pid == -1)
    {
        ft_putstr_fd("minishell: fork: ", 2);
        ft_putendl_fd(strerror(errno), 2);
        close(fd);
        return 0;
    }
    else if (pid == 0) // Child process
    {
        // Redirect stdout to file
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            ft_putstr_fd("minishell: dup2: ", 2);
            ft_putendl_fd(strerror(errno), 2);
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
        
        // Create command struct just for the command part (not including the redirection part)
        t_token *cmd_only = NULL;
        t_token *curr = tk;
        t_token *last = NULL;
        
        // Copy tokens up to the redirection operator
        while (curr && curr != redirect_token)
        {
            t_token *new_node = (t_token *)malloc(sizeof(t_token));
            if (!new_node)
                exit(EXIT_FAILURE);
                
            new_node->cmd = ft_strdup(curr->cmd);
            new_node->type = curr->type;
            new_node->next = NULL;
            
            if (!cmd_only)
                cmd_only = new_node;
            else
                last->next = new_node;
                
            last = new_node;
            curr = curr->next;
        }
        
        // If it's a builtin, handle with our truncated command list
        if (ft_is_builtin(cmd_only))
        {
            handle_builtin(cmd_only, ft_env, env, NULL);
            free_token_list(cmd_only);
            exit(EXIT_SUCCESS);
        }
        
        // Otherwise execute as external command
        if (cmd_only && cmd_only->cmd)
        {
            char **args = ft_split(cmd_only->cmd, ' ');
            if (!args)
                exit(EXIT_FAILURE);
                
            char *path = get_path(args[0], ft_env);
            if (!path)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(args[0], 2);
                ft_putendl_fd(": command not found", 2);
                ft_free_tab(args);
                free_token_list(cmd_only);
                exit(EXIT_FAILURE);
            }
            
            if (execve(path, args, ft_env) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(args[0], 2);
                ft_putstr_fd(": ", 2);
                ft_putendl_fd(strerror(errno), 2);
                ft_free_tab(args);
                free(path);
                free_token_list(cmd_only);
                exit(EXIT_FAILURE);
            }
        }
        
        free_token_list(cmd_only);
        exit(EXIT_FAILURE);
    }
    else // Parent process
    {
        close(fd);
        waitpid(pid, &status, 0);
    }
    
    return 1;
}
int handle_great(char *filename, t_token *tk, char **ft_env, t_env *env)
{
    int fd;
    pid_t pid;
    int status;
    char **args = NULL;
    char *path = NULL;
    
    // Open the output file with proper permissions
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(filename, 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd(strerror(errno), 2);
        return 0;
    }
    
    // Find the command before the '>' token
    t_token *cmd_tokens = NULL;
    t_token *current = tk;
    t_token *last_cmd = NULL;
    
    // Collect all tokens before the '>' token
    while (current && current->cmd && ft_strcmp(current->cmd, ">") != 0)
    {
        t_token *new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            close(fd);
            return 0;
        }
        new_token->cmd = ft_strdup(current->cmd);
        new_token->type = current->type;
        new_token->next = NULL;
        
        if (!cmd_tokens)
            cmd_tokens = new_token;
        else
            last_cmd->next = new_token;
        
        last_cmd = new_token;
        current = current->next;
    }
    
    // If no command was found, just create the file
    if (!cmd_tokens)
    {
        close(fd);
        return 1;
    }
    
    // Create argument list for execve
    if (cmd_tokens->cmd)
    {
        // For simple commands like "cat l"
        if (cmd_tokens->next && cmd_tokens->next->cmd)
        {
            // We have at least a command and an argument
            // Need to build an array of arguments
            int arg_count = 0;
            t_token *count_token = cmd_tokens;
            while (count_token)
            {
                arg_count++;
                count_token = count_token->next;
            }
            
            args = malloc(sizeof(char*) * (arg_count + 1));
            if (!args)
            {
                close(fd);
                free_token_list(cmd_tokens);
                return 0;
            }
            
            int i = 0;
            t_token *arg_token = cmd_tokens;
            while (arg_token)
            {
                args[i++] = ft_strdup(arg_token->cmd);
                arg_token = arg_token->next;
            }
            args[i] = NULL;
        }
        else
        {
            // Simple command with no arguments
            args = malloc(sizeof(char*) * 2);
            if (!args)
            {
                close(fd);
                free_token_list(cmd_tokens);
                return 0;
            }
            args[0] = ft_strdup(cmd_tokens->cmd);
            args[1] = NULL;
        }
    }
    
    // Execute command with redirection
    pid = fork();
    if (pid == -1)
    {
        ft_putstr_fd("minishell: fork: ", 2);
        ft_putendl_fd(strerror(errno), 2);
        close(fd);
        free_token_list(cmd_tokens);
        if (args) ft_free_tab(args);
        return 0;
    }
    else if (pid == 0) // Child process
    {
        // Redirect stdout to file
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            ft_putstr_fd("minishell: dup2: ", 2);
            ft_putendl_fd(strerror(errno), 2);
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
        
        // Check if it's a builtin
        if (ft_is_builtin(cmd_tokens))
        {
            handle_builtin(cmd_tokens, ft_env, env, NULL);
            free_token_list(cmd_tokens);
            if (args) ft_free_tab(args);
            exit(EXIT_SUCCESS);
        }
        
        // Try to execute as external command
        if (args && args[0])
        {
            path = get_path(args[0], ft_env);
            if (!path)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(args[0], 2);
                ft_putendl_fd(": command not found", 2);
                free_token_list(cmd_tokens);
                ft_free_tab(args);
                exit(EXIT_FAILURE);
            }
            
            execve(path, args, ft_env);
            
            // If execve fails
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(args[0], 2);
            ft_putstr_fd(": ", 2);
            ft_putendl_fd(strerror(errno), 2);
            free(path);
        }
        
        free_token_list(cmd_tokens);
        if (args) ft_free_tab(args);
        exit(EXIT_FAILURE);
    }
    else // Parent process
    {
        close(fd);
        waitpid(pid, &status, 0);
        free_token_list(cmd_tokens);
        if (args) ft_free_tab(args);
    }
    
    return 1;
}

// Function to process command with append redirection
int process_append_redirection(t_token *tk, char **ft_env, t_env *env)
{
    if (!tk)
        return 0;

    // Find if there's an append redirection
    t_token *curr = tk;
    t_token *redir_token = NULL;
    char *filename = NULL;

    while (curr)
    {
        if (curr->cmd && ft_strcmp(curr->cmd, ">>") == 0 && curr->next && curr->next->cmd)
        {
            redir_token = curr;
            filename = curr->next->cmd;
            break;
        }
        curr = curr->next;
    }

    if (redir_token && filename)
    {
        return handle_dgreat(filename, tk, ft_env, env);
    }

    return 0; // No redirection found
}