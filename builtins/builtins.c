/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/13 20:59:19 by njoudieh42       ###   ########.fr       */
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
		if (curr->cmd[1] != '-' || curr->cmd[2])
		{
			ft_putstr_fd("bash pwd: -", 2);
			ft_putchar_fd(curr->cmd[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			g_minishell.env->exit_status = 1;
			return (1);
		}
	}
	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		g_minishell.env->exit_status = ENU_SUCCESS;
		return (1);
	}
	else
	{
		char *pwd_env = get_value_from_env("PWD", g_minishell.env);
		if (pwd_env && pwd_env[0])
		{
			if (pwd_env)
			{
				ft_putendl_fd(pwd_env, 1);
				g_minishell.env->exit_status = ENU_SUCCESS;
				return (1);
			}
		}
		g_minishell.env->exit_status = ENU_GENEREAL_FAILURE;
	}
	return (0);
}

void	ft_print_env(t_env *env, char **envp)
{
	t_env	*ev;
	char	*pwd;

	ev = env;
	if (!ev)
	{
		ft_putstr_fd(get_value_from_env("_=", env), 1);
		ft_putstr_fd("\n", 1);
		pwd = ft_get_pwd();
		if (pwd)
		{
			char *pwd_env = ft_strjoin("PWD=", pwd);
			if (pwd_env)
			{
				ft_putstr_fd(pwd_env, 1);
				ft_putstr_fd("\n", 1);
				free(pwd_env);
			}
			free(pwd);
		}
		ft_putstr_fd("SHLVL=1\n", 1);
		ft_putstr_fd("_=\n", 1);
	}
	else
	{
		while (ev)
		{
			ft_putstr_fd(ev->line, 1);
			ft_putstr_fd("\n", 1);
			ev = ev->next;
		}
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
	remove_added_quotes(&tk->cmd);
	if (!ft_strcmp(tk->cmd, "env"))
		ft_print_env(g_minishell.env, ft_env);
	else if (!ft_strcmp(tk->cmd, "pwd"))
		ft_pwd(tk);
	else if (!ft_strcmp(tk->cmd, "cd"))
		ft_cd(tk, *env, ft_env);
	else if (!ft_strcmp(tk->cmd, "exit"))
		ft_exit(tk, &copy);
	else if (!ft_strcmp(tk->cmd, "export"))
		ft_export(tk, &copy);
	else if (!ft_strcmp(tk->cmd, "unset"))
		ft_unset(tk, &g_minishell.env, &copy);
	else if (!ft_strcmp(tk->cmd, "echo"))
		ft_echo(tk);
}
