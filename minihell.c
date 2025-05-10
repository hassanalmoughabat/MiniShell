/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#             */
/*   Updated: 2025/04/29 23:44:06 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"
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
	shell->shell_level = get_shell_level(shell->env);
	update_shlvl_in_env(&shell->env, shell->shell_level);
	if (!(shell->env))
	{
		ft_putstr_fd("failed\n", 1);
		exit(1);
	}
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
	ft_printf("%s#%s		%sWelcome To Minihell!%s	        %s#%s\n",
		yellow, reset, cyan, reset, yellow, reset);
	ft_printf("%s#%s						%s#%s\n",
		yellow, reset, yellow, reset);
	ft_printf("%s#%s       %sA Small Copy of Hell is With You!%s	%s#%s\n",
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
		input = readline("minishell:~$");
		if (!input)
			break ;
		else
		{
			inp = ft_tokenize(input);
			shell_handling(inp);
			if (*ftenv)
				free_array(ftenv);
			ftenv = transform(g_minishell.env);
			replace_dollar(&inp);
			// if (inp)
			// 	display_list(inp);
			if (inp)
				after_parsing(inp, ftenv, &env, input);
			if (inp)
				free_token_list(inp);
			add_history(input);
		}
	}
	return (0);
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
	if (path)
		free(path);
	if (ft_transform)
		free_array(ft_transform);
	ft_printf("exit \n");
	return (0);
}
