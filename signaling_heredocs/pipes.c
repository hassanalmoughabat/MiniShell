/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:36:57 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/10 17:56:26 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int has_heredoc(t_token *lst)
{
    int             i;
    t_token *curr;

    curr = lst;
    i = 0;
    while (curr)
    {
        if (curr->type == T_DLESS)
            i++;
        curr = curr->next;
    }
    return (i);
}

static void close_all_pipes(int **fds, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        close(fds[i][0]);
        close(fds[i][1]);
        i++;
    }
}

static t_token *next_pipe(t_token *start)
{
    while (start && start->type != T_PIPE)
        start = start->next;
    return (start);
}

void    handle_pipe(t_token *lst, char **ft_env, t_env *env)
{
    int             pipes_count;
    int             **fds;
    t_token *curr;
    t_token *start;
    int             i;
    pid_t   pid;
    int             status;

    if (!lst)
        return ;
    if (has_heredoc(lst))
        handle_heredoc(ft_env, env, lst);
    pipes_count = 0;
    curr = lst;
    while (curr)
    {
        if (curr->type == T_PIPE)
            pipes_count++;
        curr = curr->next;
    }
    fds = malloc(sizeof(int *) * (pipes_count));
    i = 0;
    while (i < pipes_count)
    {
        fds[i] = malloc(sizeof(int) * 2);
        if (pipe(fds[i]) == -1)
            return ;
        i++;
    }
    start = lst;
    curr = next_pipe(start);
    i = 0;
    while (1)
    {
        t_token *cmd_tokens;
        cmd_tokens = copy_tokens_before_redirect(start, curr);
        pid = fork();
        if (pid == 0)
        {
            if (i > 0)
                dup2(fds[i - 1][0], STDIN_FILENO);
            if (i < pipes_count)
                dup2(fds[i][1], STDOUT_FILENO);
            close_all_pipes(fds, pipes_count);
            if (ft_is_builtin(cmd_tokens))
            {
                handle_builtin(cmd_tokens, ft_env, &env);
                free_token_list(cmd_tokens);
                exit(EXIT_SUCCESS);
            }
            else
                execute_external_cmd(cmd_tokens, ft_env);
        }
        free_token_list(cmd_tokens);
        if (i > 0)
        {
            close(fds[i - 1][0]);
            close(fds[i - 1][1]);
        }
        if (!curr)
            break ;
        start = curr->next;
        curr = next_pipe(start);
        i++;
    }
    close_all_pipes(fds, pipes_count);
    while (wait(&status) > 0)
        ;
    while (pipes_count-- > 0)
        free(fds[pipes_count]);
    free(fds);
    if (WIFEXITED(status))
        env->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        env->exit_status = 128 + WTERMSIG(status);
}
