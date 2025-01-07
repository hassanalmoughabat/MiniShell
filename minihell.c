/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/01/07 14:11:08 by hal-moug         ###   ########.fr       */
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

#include <stdio.h>

void print_welcome_message()
{
    // ANSI Color Codes
    const char *RESET;
    const char *RED;
    const char *YELLOW;
    const char *CYAN;

	RESET= "\033[0m";
	RED = "\033[31m";
	YELLOW = "\033[33m";
	CYAN = "\033[36m";
    ft_printf("\n");
    ft_printf("%s######################################################%s\n", RED, RESET);
    ft_printf("%s#%s            %sWelcome to minihell!%s             %s#%s\n", YELLOW, RESET, CYAN, RESET, YELLOW, RESET);
    ft_printf("%s#%s       %sA small copy of Hell is with you!%s      %s#%s\n", YELLOW, RESET, CYAN, RESET, YELLOW, RESET);
    ft_printf("%s######################################################%s\n", RED, RESET);
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
