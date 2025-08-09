/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/06 23:33:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

static int	handle_pwd_options(t_token *curr, t_env *env)
{
	int	i;

	while (curr && curr->cmd[0] == '-')
	{
		if (ft_strcmp(curr->cmd, "--") == 0)
			break ;
		i = 1;
		while (curr->cmd[i])
		{
			if (curr->cmd[i] != 'L' && curr->cmd[i] != 'P')
			{
				ft_putstr_fd("minishell: pwd: -", 2);
				ft_putchar_fd(curr->cmd[i], 2);
				ft_putstr_fd(": invalid option\n", 2);
				ft_putstr_fd("pwd: usage: pwd [-LP]\n", 2);
				env->exit_status = 2;
				return (0);
			}
			i++;
		}
		curr = curr->next;
	}
	return (1);
}

int	ft_pwd(t_shell *shell)
{
	t_token	*curr;
	char	*pwd_env;
	char	cwd[PATH_MAX];

	curr = shell->tk;
	if (curr && ft_strcmp(curr->cmd, "pwd") == 0)
		curr = curr->next;
	if (!handle_pwd_options(curr, shell->env))
		return (1);
	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (shell->env->exit_status = 0, 1);
	}
	pwd_env = get_value_from_env("PWD", shell->env);
	if (pwd_env && pwd_env[0])
	{
		ft_putendl_fd(pwd_env, 1);
		free(pwd_env);
		return (shell->env->exit_status = 0, 1);
	}
	ft_putstr_fd("minishell: pwd: error retrieving current directory\n", 2);
	if (pwd_env)
		free(pwd_env);
	return (shell->env->exit_status = 1, 1);
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
	remove_added_quotes(&shell->tk->cmd);
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
