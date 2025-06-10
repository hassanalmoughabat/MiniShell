/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:36:57 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/10 19:38:52 by hal-moug         ###   ########.fr       */
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



static int is_heredoc_pipe_redirect(t_token *tk)
{
    t_token *curr;

    if (!tk || tk->type != T_DLESS || !tk->next)
        return (0);
    curr = tk->next->next;
    if (!curr || curr->type != T_PIPE)
        return (0);
    curr = curr->next;
    while (curr)
    {
        if ((curr->type == T_GREAT || curr->type == T_DGREAT) && curr->next)
            return (1);
        curr = curr->next;
    }
    return (0);
}

static void handle_hd_pipe_redir(t_token *lst, char **ft_env, t_env *env)
{
    char    *delimiter;
    int             quote;
    int             hd_fd;
    t_token *pipe_token;
    t_token *redir_token;
    int             out_fd;
    int             append;
    t_token *cmd_tokens;
    pid_t   pid;
    int             status;

    delimiter = get_delimeter(lst);
    if (!delimiter)
        return ;
    quote = is_delimeter_quoted(lst);
    hd_fd = handle_dless(delimiter, env, 0, quote);
    free(delimiter);
    if (hd_fd < 0)
        return ;
    pipe_token = next_pipe(lst);
    redir_token = find_redirect_token(pipe_token->next, ">>");
    append = 0;
    if (redir_token)
        append = 1;
    else
        redir_token = find_redirect_token(pipe_token->next, ">");
    if (!redir_token || !redir_token->next)
    {
        close(hd_fd);
        return ;
    }
    out_fd = open_output_file(redir_token->next->cmd,
            append ? O_WRONLY | O_CREAT | O_APPEND
                   : O_WRONLY | O_CREAT | O_TRUNC);
    if (out_fd == -1)
    {
        close(hd_fd);
        return ;
    }
    cmd_tokens = copy_tokens_before_redirect(pipe_token->next, redir_token);
    pid = fork();
    if (pid == 0)
    {
        dup2(hd_fd, STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        close(hd_fd);
        close(out_fd);
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
    close(hd_fd);
    close(out_fd);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        env->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        env->exit_status = 128 + WTERMSIG(status);
}


static bool     cat_before(t_token *start, t_token *here)
{
    while (start && start != here)
    {
        if (start->cmd && ft_strcmp(start->cmd, "cat") == 0)
            return (true);
        start = start->next;
    }
    return (false);
}

static char     *get_append_file(t_token *curr)
{
    while (curr)
    {
        if (curr->type == T_DGREAT && curr->next && curr->next->type == T_IDENTIFIER)
            return (curr->next->cmd);
        curr = curr->next;
    }
    return (NULL);
}

static int cat_exit_before_heredoc(t_token *lst)
{
    t_token *curr;
    int flag;
    
    curr = lst;
    flag = 0;
    while (curr)
    {
        if (ft_strcmp(curr->cmd, "<<") == 0)
            if (ft_strcmp(curr->cmd, "cat") == 0)
                flag = 1;
       curr = curr->next; 
    }
    if (flag == 1)
        return (1);
    else
        return (0);
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
       if (is_heredoc_pipe_redirect(lst) && cat_exit_before_heredoc(lst))
    {
        handle_hd_pipe_redir(lst, ft_env, env);
        return ;
    }
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
