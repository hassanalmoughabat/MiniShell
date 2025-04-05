/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/04/05 22:07:27 by njoudieh42       ###   ########.fr       */
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

void	replace_dollar(t_token **t_list, t_env *env)
{
	t_token		*curr;
	char		*temp;

	if (!t_list || !*t_list)
		return ;
	curr = *t_list;
	temp = NULL;
	while (curr)
	{
		if (ft_has_dollar(curr->cmd))
		{
			handle_value(curr->cmd, &temp, env);
			if (temp)
			{
				free(curr->cmd);
				curr->cmd = ft_strdup(temp);
			}
			else
			{
				free(curr->cmd);
				curr->cmd = ft_strdup("");
			}
		}
		curr = curr->next;
	}
}

int	ft_read(char *input, char **ftenv, t_env *env)
{
	t_token	*inp;

	print_welcome_message();
	while (1)
	{
		input = readline("minishell>");
		if (!input)
			break ;
		else
		{
			inp = ft_tokenize(input);
			replace_dollar(&inp, env);
			display_list(inp);
			if (inp)
				after_parsing(inp, ftenv, &env, input);
			add_history(input);
		}
		free(inp);
		free(input);
	}
	free (input);
	return (0);
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
	ft_read(input, ft_transform, shell.env);
	printf("end\n");
	return (0);
}
