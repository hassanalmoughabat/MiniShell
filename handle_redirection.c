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
int handle_great(char *filename)
{
    int fd;

    if (!filename || *filename == '\0')
    {
        error_print("the filename is empty", 0);
        return 0;
    }
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (!fd)
    {
        error_print("error creating the file", 0);
        return 0;
    }
    return (1);
    /* Creates a new file if it doesn't exist
    If file exists, it DELETES ALL EXISTING CONTENt Always starts with a clean /
        empty file Destructive operation(removes previous content)*/
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_HEREDOC_SIZE 16384 // 16KB limit to prevent memory exhaustion

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_HEREDOC_SIZE 16384

static int validate_delimiter(const char *delimiter)
{
    if (!delimiter || *delimiter == '\0')
    {
        errno = EINVAL;
        return 0;
    }
    return 1;
}

int handle_dless(char *delimiter)
{
    int pipefd[2];
    ssize_t total_written = 0;
    char *line;

    if (!validate_delimiter(delimiter))
        return -1;
    if (pipe(pipefd) == -1)
    {
        perror("pipe creation failed");
        return -1;
    }
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        size_t line_len = strlen(line);
        if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
        {
            free(line);
            close(pipefd[0]);
            close(pipefd[1]);
            errno = E2BIG;
            return -1;
        }
        write(pipefd[1], line, line_len);
        write(pipefd[1], "\n", 1);
        total_written += line_len + 1;
        free(line);
    }
    close(pipefd[1]);
    return pipefd[0];
}

    int handle_redirection(t_token *tk, char **ft_env, t_env *env)
{
    t_token *curr;

    curr = tk;
    while (curr)
    {
        if (curr->type == T_DGREAT || curr->type == T_GREAT)
        {
            if (curr->next && curr->next->type == T_IDENTIFIER)
            {
                if (curr->type == T_DGREAT)
                    handle_dgreat(curr->next->cmd);
                else
                    handle_great(curr->next->cmd);
            }
        }
        else if (curr->type == T_DLESS || curr->type == T_LESS)
        {
            if (curr->next && curr->next->type == T_IDENTIFIER)
            {
                if (curr->type == T_DLESS)
                    handle_dless(curr->next->cmd);
                // else
                //     handle_less(curr->next->cmd);
            }
        }
        curr = curr->next;
    }
    return (1);
}
