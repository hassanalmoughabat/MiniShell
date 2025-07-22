/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 15:44:27 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_pwd_option(char *curr, int flag, t_env *env)
{
	if (flag == 1 && (curr[1] != '-' || curr[2]))
	{
		ft_putstr_fd("bash pwd: -", 2);
		ft_putchar_fd(curr[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		env->exit_status = 1;
	}
	else if (flag == 2)
	{
		if (curr && curr[0])
		{
			ft_putendl_fd(curr, 1);
			env->exit_status = ENU_SUCCESS;
			return ;
		}
		env->exit_status = ENU_GENEREAL_FAILURE;
	}
	return ;
}

int	ft_pwd(t_shell *shell)
{
	t_token	*curr;
	char	*pwd_env;
	char	cwd[PATH_MAX];

	curr = shell->tk;
	pwd_env = get_value_from_env("PWD", shell->env);
	if (curr->next)
		curr = curr->next;
	if (curr->cmd[0] == '-')
		return (ft_pwd_option(curr->cmd, 1, shell->env), 1);
	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		shell->env->exit_status = ENU_SUCCESS;
		return (1);
	}
	else
		ft_pwd_option(pwd_env, 2, shell->env);
	return (0);
}

void	print_helper(t_env *env)
{
	char	*pwd;
	char	*pwd_env;

	ft_putstr_fd(get_value_from_env("_=", env), 1);
	ft_putstr_fd("\n", 1);
	pwd = ft_get_pwd();
	if (pwd)
	{
		pwd_env = ft_strjoin("PWD=", pwd);
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

void	ft_print_env(t_env *env)
{
	t_env	*ev;

	ev = env;
	if (!ev)
		print_helper(env);
	else
	{
		while (ev)
		{
			ft_putstr_fd(ev->line, 1);
			ft_putstr_fd("\n", 1);
			ev = ev->next;
		}
	}
	env->exit_status = ENU_SUCCESS;
}

t_env	*ft_copy(t_env *envp)
{
	t_env	*head;
	t_env	*curr1;
	t_env	*new_node;
	t_env	*current;

	head = NULL;
	current = envp;
	while (current)
	{
		new_node = new_env_node(current->line);
		if (!new_node)
			return (ft_free_env(head), NULL);
		if (!head)
		{
			head = new_node;
			curr1 = new_node;
		}
		else
		{
			curr1->next = new_node;
			curr1 = curr1->next;
		}
		current = current->next;
	}
	return (head);
}

void	handle_builtin(t_shell *shell, char *cmd)
{
	static t_env	*copy;

	if (copy == NULL)
		copy = copy_env(shell->env);
	if (!copy)
		return ;
	remove_added_quotes(&cmd);
	if (!ft_strcmp(cmd, "env"))
		ft_print_env(shell->env);
	else if (!ft_strcmp(cmd, "pwd"))
		ft_pwd(shell);
	else if (!ft_strcmp(cmd, "cd"))
		ft_cd(shell);
	else if (!ft_strcmp(cmd, "exit"))
		ft_exit(shell);
	else if (!ft_strcmp(cmd, "export"))
		ft_export(shell, &copy);
	else if (!ft_strcmp(cmd, "unset"))
		ft_unset(shell, &copy);
	else if (!ft_strcmp(cmd, "echo"))
		ft_echo(shell);
}
