/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:20:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/16 22:10:48 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

void	handle_path_command(t_token *tk, char *envp[], char *cmd)
{
	char	**s_cmd;
	char	*path;
	pid_t	pid;
	int		status;

	if (!tk->cmd || !ft_strncmp(tk->cmd, "", 1))
		return ;
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd || !s_cmd[0])
	{
		g_minishell.env->exit_status = ft_err_msg((t_error){(tk->cmd),
				ERROR, ENU_GENEREAL_FAILURE});
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_free_tab(s_cmd);
		g_minishell.env->exit_status = ft_err_msg((t_error){(tk->cmd),
				ERROR, ENU_GENEREAL_FAILURE});
		return ;
	}
	if (pid == 0)
	{
		path = get_path(s_cmd[0], envp);
		if (!path || execve(path, s_cmd, envp) == -1)
		{
			ft_printf("i am leaking \n");
			g_minishell.env->exit_status = ft_err_msg((t_error){(tk->cmd),
					ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
			if (path)
				free(path);
			ft_free_tab(s_cmd);
			ft_free_token_list(&tk);
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_minishell.env->exit_status = WEXITSTATUS(status);
		else
			g_minishell.env->exit_status = 0;
		ft_free_tab(s_cmd);
	}
}

void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input)
{
	t_token	*curr;

	if (!tk)
		return ;
	curr = tk;
	replace_dollar(&tk);
	if (curr->type == T_PIPE)
		return ;
	else if (contain_list("<<", tk) || contain_list(">>", tk)
		|| contain_list("<", tk) || contain_list(">", tk))
		handle_redirection(tk, ft_env, *env);
	else
	{
		remove_added_quotes(&(curr->cmd));
		if (ft_is_builtin(curr))
			handle_builtin(tk, ft_env, env);
		else
			handle_path_command(curr, ft_env, input);
	}
	return ;
}
