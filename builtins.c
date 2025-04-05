/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/05 18:32:47 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (1);
	}
	else
		return (0);
}

void	ft_print_env(t_env *env)
{
	t_env	*ev;

	ev = env;
	while (ev)
	{
		ft_printf("%s\n", ev->line);
		ev = ev->next;
	}
}

void	ft_exit(t_token *tk, char **ft_env, t_env *env, char *input)
{
	int		exit_code;
	t_token	*curr;

	curr = tk->next;
	exit_code = 0;
	ft_printf("exit\n");
	if (curr && curr->cmd)
		exit_code = ft_atoi(curr->cmd);
	free_token_list(tk);
	free_env_list(env);
	free_array(ft_env);
	free(input);
	exit(exit_code % 256);
}

void	handle_builtin(t_token *tk, char **ft_env, t_env *env, char *input)
{
	static t_env	*copy;

	if (copy == NULL)
		copy = copy_env(env);
	if (!ft_strcmp(tk->cmd, "env"))
		ft_print_env(env);
	else if (!ft_strcmp(tk->cmd, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(tk->cmd, "cd"))
		ft_cd(tk, env, ft_env);
	else if (!ft_strcmp(tk->cmd, "exit"))
		ft_exit(tk, ft_env, env, input);
	else if (!ft_strcmp(tk->cmd, "export"))
		ft_export(tk, &env, &copy);
	else if (!ft_strcmp(tk->cmd, "unset"))
		ft_unset(tk, &env, &copy);
	else if (!ft_strcmp(tk->cmd, "echo"))
		ft_echo(tk, env);
}
