/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:20:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/22 14:35:25 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

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
			// ft_free_token_list(&tk);
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

void print_tk_with_type(t_token *tk)
{
	t_token *curr = tk;

	while (curr)
	{
		if (curr->type == T_PIPE)
			ft_printf("Pipe: %s\n", curr->cmd);
		else if (curr->type == T_DLESS)
			ft_printf("Redirect In: %s\n", curr->cmd);
		else if (curr->type == T_DGREAT)
			ft_printf("Redirect Out: %s\n", curr->cmd);
		else
			ft_printf("Command: %s\n", curr->cmd);
		curr = curr->next;
	}
}

void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input)
{
	t_token	*curr;
	int pipe ;
	
	if (!tk)
		return ;
	curr = tk;
	replace_dollar(&tk);
	// print_tk_with_type(tk);
	if (contain_char(input, '|'))
		pipe = 1;
	if (curr->type == T_PIPE || pipe == 1)
	{
		
		handle_pipe(tk, ft_env, *env, input);
	}
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
