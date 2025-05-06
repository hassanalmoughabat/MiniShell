#include "includes/minihell.h"



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
char *find_redirection_filename(t_token *tk, char *operator)
{
    t_token *curr = tk;
    
    while (curr)
    {
        if (curr->cmd && ft_strcmp(curr->cmd, operator) == 0 && curr->next && curr->next->cmd)
            return curr->next->cmd;
        curr = curr->next;
    }
    return NULL;
}

int handle_redirection(t_token *tk, char **ft_env, t_env *env)
{
    int result = 0;
    
    // Check for >> redirection
    if (contain_list(">>", tk))
    {
        char *filename = find_redirection_filename(tk, ">>");
        if (filename)
            result = handle_dgreat(filename, tk, ft_env, env);
        else
            ft_putendl_fd("minishell: syntax error near unexpected token `>>'", 2);
    }
    // Check for > redirection
    else if (contain_list(">", tk))
    {
        char *filename = find_redirection_filename(tk, ">");
        if (filename)
            result = handle_great(filename, tk, ft_env, env);
        else
            ft_putendl_fd("minishell: syntax error near unexpected token `>'", 2);
    }
    // Check for << heredoc
    else if (contain_list("<<", tk))
    {
        handle_cat_heredoc(ft_env, env, tk);
        result = 1;
    }
    // Check for < input redirection (if implemented)
    else if (contain_list("<", tk))
    {
        // Not fully implemented yet
        result = 1;
    }
    
    return result;
}
