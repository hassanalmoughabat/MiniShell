/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/13 21:23:12 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
t_shell	g_minishell;

int	get_shell_level(t_env *shell)
{
	t_env	*curr;
	char	*temp;
	char	*val;
	int		shell_val;

	curr = shell;
	shell_val = 1;
	while (curr && ft_strncmp(curr->line, "SHLVL=", 6))
		curr = curr->next;
	if (!curr)
		return (1);
	temp = ft_strdup(curr->line);
	val = ft_strchr(temp, '=');
	shell_val = ft_atoi(val + 1) + 1;
	free (temp);
	return (shell_val);
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = initialize_env_list(envp);
	shell->path = NULL;
	if (!shell->env)
	{
		char *pwd = ft_get_pwd();
		if (pwd)
			ft_add_env("PWD", pwd, &shell->env, 0);
		ft_add_env("SHLVL", "1", &shell->env, 0);
		ft_add_env("_", "/usr/bin/env", &shell->env, 0);
		shell->shell_level = get_shell_level(shell->env);
		return ;
	}
	else
	{
		char *pwd_value = get_value_from_env("PWD", shell->env);
		if (!pwd_value || !ft_strcmp(pwd_value, ""))
		{
			char *pwd = ft_get_pwd();
			if (pwd)
			{
				ft_add_env("PWD", pwd, &shell->env, 0);
				free(pwd);
			}
		}
		free(pwd_value);
	}
	update_env_value(&(shell->env), "_=", "/usr/bin/env");
	shell->shell_level = get_shell_level(shell->env);
}

void	print_welcome_message(void)
{
	const char	*reset;
	const char	*red;
	const char	*yellow;
	const char	*cyan;

	reset = "\033[0m";
	red = "\033[31m";
	yellow = "\033[33m";
	cyan = "\033[36m";
	ft_putstr_fd("\n", 1);
	ft_printf("%s#################################################%s\n",
		red, reset);
	ft_printf("%s#%s		%sWelcome To MiniHell!%s	        %s#%s\n",
		yellow, reset, cyan, reset, yellow, reset);
	ft_printf("%s#%s						%s#%s\n",
		yellow, reset, yellow, reset);
	ft_printf("%s#%s       %sA Small Copy Of Hell Is With You!%s	%s#%s\n",
		yellow, reset, cyan, reset, yellow, reset);
	ft_printf("%s#%s						%s#%s\n",
		yellow, reset, yellow, reset);
	ft_printf("%s#%s      %sH.MOUGHGABAT	&&	N.JOUDIEH%s	%s#%s\n",
		yellow, reset, cyan, reset, yellow, reset);
	ft_printf("%s#################################################%s\n",
		red, reset);
	ft_putstr_fd("\n", 1);
}

int	ft_read(char *input, char **ftenv, t_env *env)
{
	t_token	*inp;

	print_welcome_message();
	while (1)
	{
		ft_init_signals();
		input = readline("minishell:~$ ");
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		else
		{
			inp = ft_tokenize(input);
			if (*ftenv)
				free_array(ftenv);
			ftenv = transform(env);
			replace_dollar(&inp);
			if (inp)
				after_parsing(inp, ftenv, &env, input);
			add_history(input);
		}
	}
	return (env->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char		**ft_transform;
	char		*input;
	char		*path;

	(void)argc;
	(void)argv;
	input = NULL;
	init_shell(&g_minishell, envp);
	ft_transform = transform(g_minishell.env);
	path = get_my_path(g_minishell.env);
	if (!path)
		g_minishell.env->exit_status = ENU_GENEREAL_FAILURE;
	else
		g_minishell.path = path;
	ft_read(input, ft_transform, g_minishell.env);
	return (g_minishell.env->exit_status);
}
