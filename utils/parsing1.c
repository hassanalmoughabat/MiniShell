/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:20:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/09 00:26:58 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	fill_argv_from_tokens(char **argv, t_token *tk, int count)
{
	char	**split;
	int		i;

	i = 0;
	while (tk && i < count)
	{
		if (ft_strchr(tk->cmd, ' '))
		{
			split = ft_split(tk->cmd, ' ');
			if (split && !add_split_parts(argv, &i, split, count))
				return (ft_free_tab(split), 0);
			ft_free_tab(split);
		}
		else if (!add_token_to_argv(argv, &i, tk))
			return (0);
		tk = tk->next;
	}
	argv[i] = NULL;
	return (1);
}

void	exit_with_cleanup(t_shell *shell, char **argv, char *cmd, int code)
{
	free_shell_args(argv);
	shell->env->exit_status = ft_err_msg((t_error){cmd, ERROR, code});
}

void	handle_path_command(t_shell *shell, char *cmd)
{
	char		**argv;
	pid_t		pid;

	if (!cmd || !shell->tk || handle_empty_cmd(shell, cmd))
		return ;
	remove_added_quotes(&cmd);
	argv = build_argv_from_tokens(shell->tk);
	if (!argv || !argv[0] || !argv[0][0])
		return (exit_with_cleanup(shell, argv, cmd, ENU_GENEREAL_FAILURE));
	g_signal.signint_child = true;
	pid = fork();
	if (pid == -1)
		return (exit_with_cleanup(shell, argv, cmd, ENU_GENEREAL_FAILURE));
	if (pid == 0)
		child_exec(shell, cmd, argv);
	else
		parent_wait_and_cleanup(shell, pid, argv);
}

void	handle_redirections(t_shell *shell, t_token *curr, char *input)
{
	pid_t	pid;
	int		status;

	g_signal.signint_child = true;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		handle_redirection(curr, shell, input);
		exit(shell->env->exit_status);
	}
	waitpid(pid, &status, 0);
	g_signal.signint_child = false;
	if (WIFSIGNALED(status))
		shell->env->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		shell->env->exit_status = WEXITSTATUS(status);
}

void	after_parsing(t_shell *shell, char *input)
{
	t_token	*curr;

	if (!shell->tk)
		return ;
	curr = shell->tk;
	if (!ft_strcmp(curr->cmd, ":") || !ft_strcmp(curr->cmd, "!"))
	{
		if (!ft_strcmp(curr->cmd, "!"))
			shell->env->exit_status = 1;
		return ;
	}
	if (handle_semicolon_error(shell, curr))
		return ;
	if (contain_list("|", curr))
		handle_pipe(shell->tk, shell, input);
	else if (contain_list("<<", curr) || contain_list(">>", curr)
		|| contain_list("<", curr) || contain_list(">", curr))
		handle_redirections(shell, curr, input);
	else if (ft_is_builtin(curr->cmd))
		handle_builtin(shell, curr->cmd);
	else
		handle_path_command(shell, curr->cmd);
	return ;
}
