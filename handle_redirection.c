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

int contain_list(char *str, t_token *tk)
{
    t_token *curr;

    curr = tk;
    while (curr)
    {
        if (strcmp(curr->cmd, str) == 0)
            return (1);
        curr = curr->next;
    }
    return (0);
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
                if (contain_list("<<", tk) == 1)
                {
                    handle_cat_heredoc(ft_env, env, tk);
                }
                // else
                //     handle_less(curr->next->cmd);
            }
        }
        curr = curr->next;
    }
    return (1);
}
