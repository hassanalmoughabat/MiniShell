/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/27 01:15:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

t_signal	g_signal;

void	print_welcome_message(void)
{
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("\033[31m", 1);
	ft_putstr_fd("#################################################\n", 1);
	ft_putstr_fd("\033[0m", 1);
	ft_putstr_fd("\033[33m#\033[0m\t\t\033[36mWelcome ", 1);
	ft_putstr_fd("To MiniHell!\033[0m\t        \033[33m#\033[0m\n", 1);
	ft_putstr_fd("\033[33m#\033[0m\t\t\t\t\t\033[33m        #\033[0m\n", 1);
	ft_putstr_fd("\033[33m#\033[0m       ", 1);
	ft_putstr_fd("\033[36mA Small Copy Of Hell Is With You!\033[0m", 1);
	ft_putstr_fd("\t\033[33m#\033[0m\n", 1);
	ft_putstr_fd("\033[33m#\033[0m\t\t\t\t\t\033[33m        #\033[0m\n", 1);
	ft_putstr_fd("\033[33m#\033[0m      \033[36mH.MOUGHGABAT\t&&\t", 1);
	ft_putstr_fd("N.JOUDIEH\033[0m\t\033[33m#\033[0m\n", 1);
	ft_putstr_fd("\033[31m############################", 1);
	ft_putstr_fd("#####################\033[0m\n", 1);
	ft_putstr_fd("\n", 1);
}

int	ft_read(char *input, t_shell *shell)
{
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
			shell->tk = ft_tokenize(input);
			if (shell->ft_env)
				free_array(shell->ft_env);
			shell->ft_env = transform(shell->env);
			replace_dollar(&(shell->tk), shell);
			if (shell->tk)
				after_parsing(shell, input);
			add_history(input);
			free(input);
			shell->tk = NULL;
		}
	}
	return (shell->env->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char		**ft_transform;
	t_shell		shell;
	char		*input;
	char		*path;
	int			exit_status;

	(void)argc;
	(void)argv;
	input = NULL;
	init_shell(&shell, envp);
	ft_transform = transform(shell.env);
	if (ft_transform)
		shell.ft_env = ft_transform;
	path = get_my_path(shell.env);
	if (!path)
		shell.env->exit_status = ENU_GENEREAL_FAILURE;
	else
		shell.path = path;
	exit_status = ft_read(input, &shell);
	return (exit_status);
}
