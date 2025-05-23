/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_great_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/05/18 17:07:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	handle_redirect_child(int fd, t_token *tk, t_token *redirect_token,
		t_redir *params)
{
	t_token	*cmd_tokens;

	cmd_tokens = copy_tokens_before_redirect(tk, redirect_token);
	if (!cmd_tokens)
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	execute_with_redirect(cmd_tokens, params->ft_env, params->env, fd);
	return (0);
}

int	handle_redirect_fork(int fd, t_token *tk, t_token *redirect_token,
		t_redir *params)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		return (0);
	}
	else if (pid == 0)
		handle_redirect_child(fd, tk, redirect_token, params);
	else
	{
		close(fd);
		waitpid(pid, &status, 0);
	}
	return (1);
}

 t_redir	*init_redir_params(char **ft_env, t_env *env)
{
	t_redir		*params;

	params = malloc(sizeof(t_redir));
	if (!params)
		return (NULL);
	params->ft_env = ft_env;
	params->env = env;
	return (params);
}

int	handle_great(char *filename, t_token *tk, char **ft_env, t_env *env)
{
	int				fd;
	int				result;
	t_token			*redirect_token;
	t_redir			*params;

	result = handle_standalone_redirect(filename, tk,
			O_WRONLY | O_CREAT | O_TRUNC);
	if (result != -1)
		return (result);
	redirect_token = find_redirect_token(tk, ">");
	fd = open_output_file(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
		return (0);
	if (!redirect_token || tk == redirect_token)
	{
		close(fd);
		return (1);
	}
	params = init_redir_params(ft_env, env);
	if (!params)
		return (close(fd), 0);
	result = handle_redirect_fork(fd, tk, redirect_token, params);
	free(params);
	return (result);
}

int	handle_dgreat(char *filename, t_token *tk, char **ft_env, t_env *env)
{
	int				fd;
	int				result;
	t_token			*redirect_token;
	t_redir			*params;

	result = handle_standalone_redirect(filename, tk,
			O_WRONLY | O_CREAT | O_APPEND);
	if (result != -1)
		return (result);
	redirect_token = find_redirect_token(tk, ">>");
	fd = open_output_file(filename, O_WRONLY | O_CREAT | O_APPEND);
	if (fd == -1)
		return (0);
	if (!redirect_token || tk == redirect_token)
	{
		close(fd);
		return (1);
	}
	params = init_redir_params(ft_env, env);
	if (!params)
		return (close(fd), 0);
	result = handle_redirect_fork(fd, tk, redirect_token, params);
	free(params);
	return (result);
}
