/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/11 01:45:53 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	ft_pwd(t_token *tk)
{
	t_token	*curr;
	char	cwd[PATH_MAX];

	curr = tk;
	if (curr->next)
		curr = curr->next;
	if (curr->cmd[0] == '-')
	{
		ft_putstr_fd("bash pwd: -", 2);
		ft_putchar_fd(curr->cmd[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_minishell.env->exit_status = 1;
		return (1);
	}
	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		g_minishell.env->exit_status = ENU_SUCCESS;
		return (1);
	}
	else
	{
		g_minishell.env->exit_status = ENU_GENEREAL_FAILURE;
		return (0);
	}
}

void	ft_print_env(t_env *env)
{
	t_env	*ev;

	ev = env;
	while (ev)
	{
		ft_putstr_fd(ev->line, 1);
		ft_putstr_fd("\n", 1);
		ev = ev->next;
	}
	g_minishell.env->exit_status = ENU_SUCCESS;
}

void	handle_builtin(t_token *tk, char **ft_env, t_env **env)
{
	static t_env	*copy;

	if (copy == NULL)
		copy = copy_env(*env);
	if (!copy)
		return ;
	// while (tk)
	// {
		if (!ft_strcmp(tk->cmd, "env"))
			ft_print_env(g_minishell.env);
		else if (!ft_strcmp(tk->cmd, "pwd"))
			ft_pwd(tk);
		else if (!ft_strcmp(tk->cmd, "cd"))
			ft_cd(tk, g_minishell.env, ft_env);
		else if (!ft_strcmp(tk->cmd, "exit"))
			ft_exit(tk, &copy);
		else if (!ft_strcmp(tk->cmd, "export"))
			ft_export(tk, &copy);
		else if (!ft_strcmp(tk->cmd, "unset"))
			ft_unset(tk, &g_minishell.env, &copy);
		else if (!ft_strcmp(tk->cmd, "echo"))
			ft_echo(tk);
	// }
}
