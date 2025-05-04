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
                    handle_great(curr->next->cmd, tk, ft_env, env);
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
