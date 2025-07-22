/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/22 21:23:08 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

t_signal	g_signal;

int	get_shell_level(t_env *shell)
{
	t_env	*curr;
	char	*temp;
	char	*val;
	int		shell_val;

	curr = shell;
	while (curr && ft_strncmp(curr->line, "SHLVL=", 6))
		curr = curr->next;
	if (!curr)
		return (1);
	temp = ft_strdup(curr->line);
	val = ft_strchr(temp, 61);
	val ++;
	shell_val = ft_atoi(val) + 1;
	printf("nbr =%d val=%s temp=%s \n", shell_val, val, temp);
	free (temp);
	return (shell_val);
}

void	ft_add(char *key, char *value, t_env **env)
{
	t_env	*new_node1;

	new_node1 = (t_env *)malloc(sizeof(t_env));
	new_node1->line = quotes_in_env(value, key, 1);
	new_node1->next = NULL;
	ft_push_to_env(env, new_node1);
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = initialize_env_list(envp);
	shell->path = NULL;
	shell->tk = NULL;
	shell->curr_cmd = NULL;
	shell->copy = initialize_env_list(envp);
	shell->ft_env = NULL;
	shell->key = NULL;
	shell->value = NULL;
	if (!shell->env)
	{
		char *pwd = ft_get_pwd();

		if (pwd)
			ft_add("PWD", pwd, &(shell->env));
		ft_add("SHLVL", "1", &(shell->env));
		ft_add("_", "/usr/bin/env", &(shell->env));
		shell->shell_level = 1;
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
				ft_add("PWD", pwd, &(shell->env));
				free(pwd);
			}
		}
		free(pwd_value);
	}
	update_env_value(&(shell->env), "_=", "/usr/bin/env");
	shell->shell_level = get_shell_level(shell->env);
	update_env_value(&(shell->env), "SHLVL=", ft_itoa(shell->shell_level));
}

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
	ft_read(input, &shell);
	return (shell.env->exit_status);
}
