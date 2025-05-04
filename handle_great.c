#include "includes/minihell.h"

int handle_dgreat(char *filename)
{

    int fd;

    if (!filename || *filename == '\0')
    {
        printf("the filename is empty");
        return 0;
    }
    fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (!fd)
    {
        printf("error creating the file");
        return (0);
    }
    return (1);
    /* it crates a file if it doesn't exist
    it preserves what was in file before and appends to end of it*/
}
// Modified handle_great function to handle command execution with redirection
// Modified handle_great function to handle command execution with redirection
int handle_great(char *filename, t_token *tk, char **ft_env, t_env *env)
{
    int fd;
    pid_t pid;
    int status;
    t_token *cmd_token = NULL;
    char *cmd = NULL;
    char *path = NULL;

    // Validate filename
    if (!filename || *filename == '\0')
    {
        error_print("the filename is empty", 0);
        return 0;
    }

    // Open file with proper flags
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        error_print("error creating the file", 0);
        return 0;
    }

    // Find the command token that comes before the '>' token
    cmd_token = tk;
    while (cmd_token)
    {
        // Make sure we don't access invalid memory
        if (!cmd_token->cmd)
        {
            cmd_token = cmd_token->next;
            continue;
        }

        // Find the token before '>'
        if (cmd_token->next && cmd_token->next->cmd &&
            ft_strcmp(cmd_token->next->cmd, ">") == 0)
        {
            cmd = cmd_token->cmd;
            break;
        }
        cmd_token = cmd_token->next;
    }

    // Safety check for cmd
    if (!cmd || ft_strcmp(cmd, ">") == 0)
    {
        close(fd);
        return 1; // Just create the file if no valid command
    }
    ;

    // Shouldn't reach here
    return 1;
}

// Function to handle redirection in the main shell
int process_output_redirection(t_token *tk, char **ft_env, t_env *env)
{
    if (!tk)
        return 0;

    // First find if there's a redirection
    t_token *curr = tk;
    t_token *redir_token = NULL;
    char *filename = NULL;

    while (curr)
    {
        if (curr->cmd && ft_strcmp(curr->cmd, ">") == 0 && curr->next && curr->next->cmd)
        {
            redir_token = curr;
            filename = curr->next->cmd;
            break;
        }
        curr = curr->next;
    }

    if (redir_token && filename)
    {
        return handle_great(filename, tk, ft_env, env);
    }

    return 0; // No redirection found
}
// Function to handle command with output redirection
static void execute_command_with_redirection(t_token *tk, char **ft_env, t_env *env)
{
    t_token *curr = tk;
    char *filename = NULL;

    // Find redirection operator
    while (curr)
    {
        if (ft_strcmp(curr->cmd, ">") == 0 && curr->next)
        {
            filename = curr->next->cmd;
            break;
        }
        curr = curr->next;
    }

    if (filename)
    {
        handle_great(filename, tk, ft_env, env);
    }
    else
    {
        // Handle normal command execution if no redirection
        // (You'll need to implement this part based on your shell's design)
    }
}