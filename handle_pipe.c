/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:30:42 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/20 18:38:24 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

/*
** Divides a token list at a pipe operator into two parts:
** - left_cmd: Commands before the pipe
** - right_cmd: Commands after the pipe
** Returns: The pipe token or NULL if not found
*/
t_token	*split_commands_at_pipe(t_token *token_list, t_token **left_cmd,
		t_token **right_cmd)
{
	t_token	*curr;
	t_token	*pipe_token;
	t_token	*new_token;
	t_token	*prev;

	curr = token_list;
	*left_cmd = NULL;
	*right_cmd = NULL;
	prev = NULL;
	while (curr && curr->type != T_PIPE)
	{
		new_token = create_token(curr->cmd, curr->type);
		if (!new_token)
			return (free_token_list(*left_cmd), NULL);
		if (!*left_cmd)
			*left_cmd = new_token;
		else
			prev->next = new_token;
		prev = new_token;
		curr = curr->next;
	}
	if (!curr || curr->type != T_PIPE)
		return (NULL);
	pipe_token = curr;
	curr = curr->next;
	while (curr)
	{
		new_token = create_token(curr->cmd, curr->type);
		if (!new_token)
			return (free_token_list(*left_cmd), free_token_list(*right_cmd), NULL);
		if (!*right_cmd)
			*right_cmd = new_token;
		else
		{
			prev = *right_cmd;
			while (prev->next)
				prev = prev->next;
			prev->next = new_token;
		}
		curr = curr->next;
	}
	return (pipe_token);
}

/*
** Sets up output redirection for a piped command
** Opens target file and redirects stdout
*/
int	setup_output_redirection(t_token *cmd, char **ft_env, t_env *env)
{
	t_token	*curr;
	char	*filename;
	int		fd;

	curr = cmd;
	while (curr)
	{
		if ((curr->type == T_GREAT || curr->type == T_DGREAT) && curr->next)
		{
			filename = curr->next->cmd;
			if (curr->type == T_GREAT)
				fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(filename, 2);
				ft_putstr_fd(": ", 2);
				ft_putendl_fd(strerror(errno), 2);
				return (0);
			}
			
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				close(fd);
				return (0);
			}
			close(fd);
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

/*
** Sets up input redirection (not heredoc) for a piped command
*/
int	setup_input_redirection(t_token *cmd)
{
	t_token	*curr;
	char	*filename;
	int		fd;

	curr = cmd;
	while (curr)
	{
		if (curr->type == T_LESS && curr->next)
		{
			filename = curr->next->cmd;
			fd = open(filename, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(filename, 2);
				ft_putstr_fd(": ", 2);
				ft_putendl_fd(strerror(errno), 2);
				return (0);
			}
			
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				close(fd);
				return (0);
			}
			close(fd);
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

/*
** Process heredoc for a piped command
** Returns 1 if heredoc was processed, 0 otherwise
*/
int	process_heredoc_pipe(t_token *cmd, t_env *env, int flag)
{
	char	*delimiter;
	int		quote;
	int		heredoc_fd;

	if (!contain_list("<<", cmd))
		return (0);
	delimiter = get_delimeter(cmd);
	if (!delimiter)
		return (0);
	quote = 0;
	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
		quote = 1;
	heredoc_fd = handle_dless(delimiter, env, flag, quote);
	if (heredoc_fd == -1)
		return (0);
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		close(heredoc_fd);
		return (0);
	}
	close(heredoc_fd);
	free(delimiter);
	return (1);
}

/*
** Execute a command with redirected input/output
** Handles prioritization of commands based on type
*/
void	execute_piped_command(t_token *cmd, char **ft_env, t_env *env,
		int in_fd, int out_fd)
{
	int	heredoc_processed;
	int	input_redirected;
	int	output_redirected;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	// First handle heredocs (absolute highest priority)
	heredoc_processed = 0;
	if (contain_list("<<", cmd))
	{
		handle_heredoc(ft_env, env, cmd);
		exit(g_minishell.env->exit_status);
	}
	
	// Set up pipe file descriptors
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(out_fd);
	}
	
	// Then handle other redirections
	input_redirected = setup_input_redirection(cmd);
	output_redirected = setup_output_redirection(cmd, ft_env, env);
	
	// Finally execute the command
	if (!heredoc_processed && !input_redirected && !output_redirected)
	{
		if (ft_is_builtin(cmd))
			handle_builtin(cmd, ft_env, &env);
		else
		{
			char *input = ft_strdup(cmd->cmd);
			if (input)
			{
				remove_added_quotes(&input);
				handle_path_command(cmd, ft_env, input);
				free(input);
			}
		}
	}
	exit(g_minishell.env->exit_status);
}

/*
** Check if this command contains a heredoc
*/
int	has_heredoc(t_token *cmd)
{
	t_token *curr;
	
	curr = cmd;
	while (curr)
	{
		if (curr->type == T_DLESS)
			return (1);
		curr = curr->next;
	}
	return (0);
}

/*
** Handle heredoc specially for pipe left side
** Returns fd for the pipe read end
*/
int	handle_heredoc_pipe_left(t_token *cmd, char **ft_env, t_env *env)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	
	if (pipe(pipefd) == -1)
		return (-1);
		
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(pipefd[1]);
		
		handle_heredoc(ft_env, env, cmd);
		exit(g_minishell.env->exit_status);
	}
	
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (-1);
	}
	
	return (pipefd[0]);
}

/*
** Executes a simple pipe between two commands
** Sets up pipe and forks processes for both sides
*/
void	handle_simple_pipe(t_token *left_cmd, t_token *right_cmd,
		char **ft_env, t_env *env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;
	int		heredoc_fd;

	// Special handling if left side has heredoc
	if (has_heredoc(left_cmd))
	{
		heredoc_fd = handle_heredoc_pipe_left(left_cmd, ft_env, env);
		if (heredoc_fd == -1)
		{
			g_minishell.env->exit_status = 1;
			return;
		}
		
		// Set up pipe for connecting to right command
		if (pipe(pipefd) == -1)
		{
			close(heredoc_fd);
			g_minishell.env->exit_status = 1;
			return;
		}
		
		// Fork for the connecting process (cat)
		left_pid = fork();
		if (left_pid == -1)
		{
			close(heredoc_fd);
			close(pipefd[0]);
			close(pipefd[1]);
			g_minishell.env->exit_status = 1;
			return;
		}
		
		if (left_pid == 0)
		{
			close(pipefd[0]);
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(heredoc_fd);
			close(pipefd[1]);
			
			// Execute cat command to pass data
			char *args[] = {"cat", NULL};
			char *path = get_path("cat", ft_env);
			execve(path, args, ft_env);
			exit(EXIT_FAILURE);
		}
		
		close(heredoc_fd);
		
		// Fork for the right command
		right_pid = fork();
		if (right_pid == -1)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			waitpid(left_pid, &status, 0);
			g_minishell.env->exit_status = 1;
			return;
		}
		
		if (right_pid == 0)
		{
			close(pipefd[1]);
			execute_piped_command(right_cmd, ft_env, env, pipefd[0], STDOUT_FILENO);
		}
		
		close(pipefd[0]);
		close(pipefd[1]);
		
		// Wait for both processes
		waitpid(left_pid, &status, 0);
		waitpid(right_pid, &status, 0);
		
		if (WIFEXITED(status))
			g_minishell.env->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_minishell.env->exit_status = 128 + WTERMSIG(status);
		
		return;
	}
	
	// Handle right side heredoc or standard pipe
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("minishell: pipe: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_minishell.env->exit_status = 1;
		return;
	}
	
	// Set signint_child flag for signal handling
	g_minishell.signint_child = true;
	
	left_pid = fork();
	if (left_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		g_minishell.signint_child = false;
		g_minishell.env->exit_status = 1;
		return;
	}
	
	if (left_pid == 0)
	{
		close(pipefd[0]);
		execute_piped_command(left_cmd, ft_env, env, STDIN_FILENO, pipefd[1]);
	}
	
	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(left_pid, &status, 0);
		g_minishell.signint_child = false;
		g_minishell.env->exit_status = 1;
		return;
	}
	
	if (right_pid == 0)
	{
		close(pipefd[1]);
		execute_piped_command(right_cmd, ft_env, env, pipefd[0], STDOUT_FILENO);
	}
	
	// Parent closes both pipe ends
	close(pipefd[0]);
	close(pipefd[1]);
	
	// Wait for both processes to complete
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	
	// Reset signal flag and set exit status
	g_minishell.signint_child = false;
	if (WIFEXITED(status))
		g_minishell.env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_minishell.env->exit_status = 128 + WTERMSIG(status);
}

/*
** Handles multiple pipes by recursively processing command segments
** For each pipe, splits commands and processes them in the correct order
*/
void	handle_multiple_pipes(t_token *tk, char **ft_env, t_env *env)
{
	t_token	*left_cmd;
	t_token	*right_cmd;
	t_token	*pipe_token;

	pipe_token = split_commands_at_pipe(tk, &left_cmd, &right_cmd);
	if (!pipe_token)
	{
		after_parsing(tk, ft_env, &env, tk->cmd);
		return ;
	}
	if (pipe_token && !right_cmd)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_minishell.env->exit_status = 2;
		free_token_list(left_cmd);
		return ;
	}
	if (contain_list("|", right_cmd))
		handle_multi_level_pipe(left_cmd, right_cmd, ft_env, env);
	else
		handle_simple_pipe(left_cmd, right_cmd, ft_env, env);
	if (left_cmd)
		free_token_list(left_cmd);
	if (right_cmd)
		free_token_list(right_cmd);
}

/*
** Process a command line containing multiple pipes
** Sets up a pipeline with multiple processes
*/
void	handle_multi_level_pipe(t_token *left_cmd, t_token *right_cmd,
		char **ft_env, t_env *env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;
	int		heredoc_fd;

	// Special handling if left side has heredoc
	if (has_heredoc(left_cmd))
	{
		heredoc_fd = handle_heredoc_pipe_left(left_cmd, ft_env, env);
		if (heredoc_fd == -1)
		{
			g_minishell.env->exit_status = 1;
			return;
		}
		
		// Set up pipe for connecting to right command
		if (pipe(pipefd) == -1)
		{
			close(heredoc_fd);
			g_minishell.env->exit_status = 1;
			return;
		}
		
		// Fork for the connecting process (cat)
		left_pid = fork();
		if (left_pid == -1)
		{
			close(heredoc_fd);
			close(pipefd[0]);
			close(pipefd[1]);
			g_minishell.env->exit_status = 1;
			return;
		}
		
		if (left_pid == 0)
		{
			close(pipefd[0]);
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(heredoc_fd);
			close(pipefd[1]);
			
			// Execute cat command to pass data
			char *args[] = {"cat", NULL};
			char *path = get_path("cat", ft_env);
			execve(path, args, ft_env);
			exit(EXIT_FAILURE);
		}
		
		close(heredoc_fd);
		
		// Fork for the right command
		right_pid = fork();
		if (right_pid == -1)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			waitpid(left_pid, &status, 0);
			g_minishell.env->exit_status = 1;
			return;
		}
		
		if (right_pid == 0)
		{
			close(pipefd[1]);
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipefd[0]);
			handle_multiple_pipes(right_cmd, ft_env, env);
			exit(g_minishell.env->exit_status);
		}
		
		close(pipefd[0]);
		close(pipefd[1]);
		
		// Wait for both processes
		waitpid(left_pid, &status, 0);
		waitpid(right_pid, &status, 0);
		
		if (WIFEXITED(status))
			g_minishell.env->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_minishell.env->exit_status = 128 + WTERMSIG(status);
		
		return;
	}
	
	// Standard pipe handling
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("minishell: pipe: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_minishell.env->exit_status = 1;
		return;
	}
	
	// Set signint_child flag for signal handling
	g_minishell.signint_child = true;
	
	left_pid = fork();
	if (left_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		g_minishell.signint_child = false;
		g_minishell.env->exit_status = 1;
		return;
	}
	
	if (left_pid == 0)
	{
		close(pipefd[0]);
		execute_piped_command(left_cmd, ft_env, env, STDIN_FILENO, pipefd[1]);
	}
	
	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(left_pid, &status, 0);
		g_minishell.signint_child = false;
		g_minishell.env->exit_status = 1;
		return;
	}
	
	if (right_pid == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(pipefd[0]);
		handle_multiple_pipes(right_cmd, ft_env, env);
		exit(g_minishell.env->exit_status);
	}
	
	// Parent closes both pipe ends
	close(pipefd[0]);
	close(pipefd[1]);
	
	// Wait for both processes to complete
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	
	// Reset signal flag and set exit status
	g_minishell.signint_child = false;
	if (WIFEXITED(status))
		g_minishell.env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_minishell.env->exit_status = 128 + WTERMSIG(status);
}

/*
** Main entry point for pipe handling
** Checks if the command line contains pipes and processes accordingly
*/
void	handle_pipe(t_token *lst, char **ft_env, t_env *env)
{
	if (!lst)
		return ;
	if (lst->type == T_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_minishell.env->exit_status = 2;
		return ;
	}
	handle_multiple_pipes(lst, ft_env, env);
}