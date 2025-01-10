/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/01/09 14:48:49 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

static	void	init_shell(t_shell *shell, char **envp)
{
	shell->env = initialize_env_list(envp);
	shell->path = NULL;
	shell->exit_status = 0;
	if (shell->env)
		ft_printf("successful\n");
	else
		ft_printf("failed\n");
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
	ft_printf("\n");
	ft_printf("%s######################################################%s\n",
		red, reset);
	ft_printf("%s#%s            %sWelcome to minihell!%s             %s#%s\n",
		yellow, reset, cyan, reset, yellow, reset);
	ft_printf("%s#%s       %sA small copy of Hell is with you!%s      %s#%s\n",
		yellow, reset, cyan, reset, yellow, reset);
	ft_printf("%s######################################################%s\n",
		red, reset);
	ft_printf("\n");
}

void	ft_read(char	*input, char **ftenv)
{
	print_welcome_message();
	while (1)
	{
		input = readline("minishell>");
		if (!input)
		{
			ft_printf("\n");
			break ;
		}
		if (input)
		{
			add_history(input);
			exec(input, ftenv);
		}
		ft_tokenize(input);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	char	*path;
	char	**ft_transform;

	(void)argc;
	(void)argv;
	ft_printf("Initializing shell...\n");
	init_shell(&shell, envp);
	ft_transform = transform(shell.env);
	ft_printf("Getting PATH...\n");
	path = get_my_path(shell.env);
	if (!path)
		ft_printf("Path is NULL\n");
	else
	{
		ft_printf("Path is: %s\n", path);
		shell.path = path;
	}
	input = NULL;
	ft_read(input, ft_transform);
	return (0);
}
