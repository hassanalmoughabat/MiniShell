/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:36:57 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/25 22:52:34 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
int has_heredoc(t_token *lst)
{
    int i;
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

static int count_pipes(t_token *lst)
{
    int count;
    t_token *curr;

    count = 0;
    curr = lst;
    while (curr)
    {
        if (curr->type == T_PIPE)
            count++;
        curr = curr->next;
    }
    return (count);
}

typedef struct s_heredoc_info
{
    int fd;
    t_token *position;
} t_heredoc_info;

static t_heredoc_info *process_heredocs_before_pipes(t_token *lst, t_env *env, int *hd_count)
{
    t_token *curr;
    t_heredoc_info *heredocs;
    int count;
    int i;
    char *delimiter;
    int quote;
    
    
    count = has_heredoc(lst);
    *hd_count = count;
    if (count == 0)
        return (NULL);
    heredocs = malloc(sizeof(t_heredoc_info) * count);
    if (!heredocs)
        return (NULL);
    curr = lst;
    i = 0;
    while (curr && i < count)
    {
        if (curr->type == T_DLESS && curr->next)
        {
            delimiter = get_delimeter(curr);
            if (!delimiter)
            {
                while (--i >= 0)
                    close(heredocs[i].fd);
                free(heredocs);
                return NULL;
            }
            
            quote = is_delimeter_quoted(curr);
            heredocs[i].fd = handle_dless(delimiter, env, 0, quote);
            heredocs[i].position = curr;
            free(delimiter);
            
            if (heredocs[i].fd < 0)
            {
                while (--i >= 0)
                    close(heredocs[i].fd);
                free(heredocs);
                return NULL;
            }
            i++;
        }
        curr = curr->next;
    }
    
    return heredocs;
}

static int	ft_pipe_builtin(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if ((ft_strcmp(curr->cmd, "cd") == 0))
			return (1);
		else if (ft_strcmp(curr->cmd, "echo") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "unset") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "env") == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

static t_token *extract_command_segment(t_token *start, t_token *end)
{
    t_token *new_list;
    t_token *curr;
    t_token *new_token;
    t_token *last;

    new_list = NULL;
    last = NULL;
    curr = start;
    while (curr && curr != end)
    {
        new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            free_token_list(new_list);
            return (NULL);
        }
        new_token->cmd = ft_strdup(curr->cmd);
        new_token->type = curr->type;
        new_token->next = NULL;
        new_token->prev = last;
        
        if (!new_list)
            new_list = new_token;
        else
            last->next = new_token;
        last = new_token;
        curr = curr->next;
    }
    return (new_list);
}

static int segment_has_heredoc(t_token *start, t_token *end)
{
    t_token *curr = start;
    
    while (curr && curr != end)
    {
        if (curr->type == T_DLESS)
            return 1;
        curr = curr->next;
    }
    return 0;
}

static int get_heredoc_fd_for_segment(t_token *start, t_token *end, 
                                     t_heredoc_info *heredocs, int hd_count)
{
    int i;
    t_token *curr = start;
    
    while (curr && curr != end)
    {
        if (curr->type == T_DLESS)
        {
            for (i = 0; i < hd_count; i++)
            {
                if (heredocs[i].position == curr)
                    return heredocs[i].fd;
            }
        }
        curr = curr->next;
    }
    return -1;
}

static void	setup_pipe_redirects(int i, int **pipes, int pipe_count,
								 int heredoc_fd, int is_first_with_heredoc)
{
	(void)is_first_with_heredoc;
	if (heredoc_fd >= 0)                              
	{
		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(heredoc_fd);
	}
	else if (i > 0)                                  
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
	}
	if (i < pipe_count)                                 
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
	}
	close_all_pipes(pipes, pipe_count);
}
static void	handle_pipe_child(t_token *cmd_segment, char **ft_env,
							   t_env *env, int i, int **pipes, int pipe_count,
							   int heredoc_fd, int is_first_with_heredoc)
{
	setup_pipe_redirects(i, pipes, pipe_count, heredoc_fd, is_first_with_heredoc);
	for (t_token *cur = cmd_segment; cur; )
	{
		if (cur->type == T_DLESS && cur->next)
		{
			t_token *next = cur->next->next;          
			if (cur->prev)
				cur->prev->next = next;
			else
				cmd_segment = next;
			if (next)
				next->prev = cur->prev;
			cur = next;
			continue;
		}
		cur = cur->next;
	}
	if (contain_list(">>", cmd_segment) || contain_list(">", cmd_segment)
		|| contain_list("<",  cmd_segment))
	{
		handle_redirection(cmd_segment, ft_env, env);
		exit(env->exit_status);
	}
	if (ft_pipe_builtin(cmd_segment))
	{
		handle_builtin(cmd_segment, ft_env, &env);
		free_token_list(cmd_segment);
		exit(EXIT_SUCCESS);
	}
	execute_external_cmd(cmd_segment, ft_env);
}


static int handle_heredoc_pipe_redirect(t_token *lst, char **ft_env, t_env *env)
{
    t_token *heredoc_token = NULL;
    t_token *pipe_token = NULL;
    t_token *redirect_token = NULL;
    t_token *curr;
    char *delimiter;
    int heredoc_fd;
    int pipefd[2];
    pid_t pid1, pid2;
    int status;
    int quote;

    curr = lst;
    while (curr)
    {
        if (curr->type == T_DLESS && !heredoc_token)
            heredoc_token = curr;
        else if (curr->type == T_PIPE && !pipe_token)
            pipe_token = curr;
        else if ((curr->type == T_GREAT || curr->type == T_DGREAT) && pipe_token)
            redirect_token = curr;
        curr = curr->next;
    }

    if (!heredoc_token || !pipe_token || !redirect_token || !redirect_token->next)
        return 0;
    delimiter = get_delimeter(lst);
    if (!delimiter)
        return 0;

    quote = is_delimeter_quoted(lst);
     if (pipe(pipefd) == -1)
    {
        free(delimiter);
        return 0;
    }
    pid1 = fork();
    if (pid1 == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        free(delimiter);
        return 0;
    }

    if (pid1 == 0)
    {
        close(pipefd[0]); 
        heredoc_fd = handle_dless(delimiter, env, 1, quote);
        if (heredoc_fd < 0)
        {
            close(pipefd[1]);
            exit(1);
        }
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        char *args[] = {"cat", NULL};
        execve("/bin/cat", args, ft_env);
        exit(127);
    }
    pid2 = fork();
    if (pid2 == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        free(delimiter);
        kill(pid1, SIGTERM);
        waitpid(pid1, NULL, 0);
        return 0;
    }

    if (pid2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        int out_fd;
        if (redirect_token->type == T_DGREAT)
            out_fd = open(redirect_token->next->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            out_fd = open(redirect_token->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (out_fd == -1)
        {
            perror("minishell");
            exit(1);
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        char *args[] = {"cat", NULL};
        execve("/bin/cat", args, ft_env);
        exit(127);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    free(delimiter);
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    if (WIFEXITED(status))
        env->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        env->exit_status = 128 + WTERMSIG(status);

    return 1;
}


static int create_pipes(int ***pipes, int pipe_count)
{
    int i;
    
    *pipes = malloc(sizeof(int *) * pipe_count);
    if (!*pipes)
        return (-1);
    i = 0;
    while (i < pipe_count)
    {
        (*pipes)[i] = malloc(sizeof(int) * 2);
        if (!(*pipes)[i])
        {
            while (--i >= 0)
                free((*pipes)[i]);
            free(*pipes);
            return (-1);
        }
        if (pipe((*pipes)[i]) == -1)
        {
            while (i >= 0)
                free((*pipes)[i--]);
            free(*pipes);
            return (-1);
        }
        i++;
    }
    return (0);
}

void handle_pipe(t_token *lst, char **ft_env, t_env *env, char *input)
{
    int pipe_count;
    int **pipes;
    t_token *curr;
    t_token *start;
    t_token *cmd_segment;
    pid_t pid;
    int i;
    int status;
    t_heredoc_info *heredocs;
    int hd_count;
    int heredoc_fd;
    int is_first_with_heredoc;

    if (!lst)
        return;
    if (has_heredoc(lst) && count_pipes(lst) == 1)
    {
        curr = lst;
        int has_heredoc_before_pipe = 0;
        int has_redirect_after_pipe = 0;
        
        while (curr && curr->type != T_PIPE)
        {
            if (curr->type == T_DLESS)
                has_heredoc_before_pipe = 1;
            curr = curr->next;
        }
        
        if (curr && curr->type == T_PIPE)
        {
            curr = curr->next;
            while (curr)
            {
                if (curr->type == T_GREAT || curr->type == T_DGREAT)
                {
                    has_redirect_after_pipe = 1;
                    break;
                }
                curr = curr->next;
            }
        }
        
        if (has_heredoc_before_pipe && has_redirect_after_pipe)
        {
            if (handle_heredoc_pipe_redirect(lst, ft_env, env))
                return;
        }
    }
    pipe_count = count_pipes(lst);
    if (pipe_count == 0)
    {
        if (has_heredoc(lst))
            handle_heredoc(ft_env, env, lst);
        else
            after_parsing(lst, ft_env, &env, input);
        return;
    }
    heredocs = process_heredocs_before_pipes(lst, env, &hd_count);
    if (create_pipes(&pipes, pipe_count) == -1)
    {
        if (heredocs)
        {
            for (i = 0; i < hd_count; i++)
                close(heredocs[i].fd);
            free(heredocs);
        }
        return;
    }
    start = lst;
    curr = next_pipe(start);
    i = 0;
    is_first_with_heredoc = 0;
    
    while (1)
    {
        heredoc_fd = -1;
        if (heredocs && segment_has_heredoc(start, curr))
        {
            heredoc_fd = get_heredoc_fd_for_segment(start, curr, heredocs, hd_count);
            if (i == 0)
                is_first_with_heredoc = 1;
        }
        cmd_segment = extract_command_segment(start, curr);
        if (!cmd_segment)
        {
            close_all_pipes(pipes, pipe_count);
            if (heredocs)
            {
                for (int j = 0; j < hd_count; j++)
                    if (heredocs[j].fd != heredoc_fd)
                        close(heredocs[j].fd);
                free(heredocs);
            }
            return;
        }
        pid = fork();
        if (pid == -1)
        {
            free_token_list(cmd_segment);
            close_all_pipes(pipes, pipe_count);
            if (heredocs)
            {
                for (int j = 0; j < hd_count; j++)
                    close(heredocs[j].fd);
                free(heredocs);
            }
            return;
        }
        
        if (pid == 0)
        {
            if (heredocs)
            {
                for (int j = 0; j < hd_count; j++)
                {
                    if (heredocs[j].fd != heredoc_fd && heredocs[j].fd >= 0)
                        close(heredocs[j].fd);
                }
            }
            
            handle_pipe_child(cmd_segment, ft_env, env, i, pipes, pipe_count,
                            heredoc_fd, is_first_with_heredoc);
            exit(EXIT_FAILURE);
        }
        if (heredoc_fd >= 0)
        {
            for (int j = 0; j < hd_count; j++)
            {
                if (heredocs[j].fd == heredoc_fd)
                {
                    close(heredocs[j].fd);
                    heredocs[j].fd = -1;
                    break;
                }
            }
        }
        free_token_list(cmd_segment);
        if (i > 0)
        {
            close(pipes[i - 1][0]);
            close(pipes[i - 1][1]);
        }
        
        if (!curr)
            break;
        
        start = curr->next;
        curr = next_pipe(start);
        i++;
    }
    close_all_pipes(pipes, pipe_count);
    if (heredocs)
    {
        for (i = 0; i < hd_count; i++)
            if (heredocs[i].fd >= 0)
                close(heredocs[i].fd);
        free(heredocs);
    }
        while (wait(&status) > 0)
        ;
    if (WIFEXITED(status))
        env->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        env->exit_status = 128 + WTERMSIG(status);
    for (i = 0; i < pipe_count; i++)
        free(pipes[i]);
    free(pipes);
}
