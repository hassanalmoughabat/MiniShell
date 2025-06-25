/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:20:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/25 22:54:04 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int		contains_symbols(char *tk, int flag)
{
	int		i;

	i = 0;
	if (flag == 1)
	{
		while (tk[i])
		{
			if ((tk[i] == '/' && !tk[i + 1])
				|| (tk[i] == '/' && !ft_isalpha(tk[i + 1])))
				return (1);
			i ++;
		}
	}
	else if (flag == 2)
	{
		while (tk[i])
		{
			if (tk[i] == '>' || tk[i] == '<' || tk[i] == '|')
				return (1);
			i ++;
		}
	}
	return (0);
}

int	count_total_args(t_token *tk)
{
	t_token	*curr;
	char	**split;
	int		total;
	int		i;

	curr = tk;
	total = 0;
	while (curr)
	{
		if (ft_strchr(curr->cmd, ' '))
		{
			split = ft_split(curr->cmd, ' ');
			if (split)
			{
				i = 0;
				while (split[i])
				{
					total++;
					i++;
				}
				ft_free_tab(split);
			}
		}
		else
			total++;
		curr = curr->next;
	}
	return (total);
}

char	**build_argv_from_tokens(t_token *tk)
{
	t_token	*curr;
	char	**argv;
	char	**split;
	int		count;
	int		i;
	int		j;

	count = count_total_args(tk);
	if (count == 0)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	curr = tk;
	i = 0;
	while (curr && i < count)
	{
		if (ft_strchr(curr->cmd, ' '))
		{
			split = ft_split(curr->cmd, ' ');
			if (split)
			{
				j = 0;
				while (split[j] && i < count)
				{
					argv[i] = ft_strdup(split[j]);
					remove_added_quotes(&argv[i]);
					if (!argv[i])
					{
						while (--i >= 0)
							free(argv[i]);
						free(argv);
						ft_free_tab(split);
						return (NULL);
					}
					i++;
					j++;
				}
				ft_free_tab(split);
			}
		}
		else
		{
			argv[i] = ft_strdup(curr->cmd);
			remove_added_quotes(&argv[i]);
			if (!argv[i])
			{
				while (--i >= 0)
					free(argv[i]);
				free(argv);
				return (NULL);
			}
			curr = curr->next;
			i++;
		}
	}
	argv[i] = NULL;
	return (argv);
}

void	handle_path_command(t_token *tk, char *envp[], char *cmd)
{
	char	**argv;
	char	*path;
	pid_t	pid;
	int		status;

	if (!cmd || !tk)
		return ;
	// display_list(tk);
	if (contains_symbols(cmd, 1))
	{
		g_minishell.env->exit_status = 127;
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(" Is a directory \n", 2);
		return ;
	}
	if (!ft_strncmp(cmd, ".//", 2) && access(cmd, X_OK))
	{
		g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
				ERROR_MESG_PERMISSION_DENIED, ENU_CMD_CANT_EXECUTE});
		return ;
	}
	argv = build_argv_from_tokens(tk);
	if (!argv || !argv[0])
	{
		remove_added_quotes(&cmd);
		if (argv)
			ft_free_tab(argv);
		g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
				ERROR, ENU_GENEREAL_FAILURE});
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_free_tab(argv);
		g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
				ERROR, ENU_GENEREAL_FAILURE});
		return ;
	}
	if (pid == 0)
	{
		path = get_path(argv[0], envp);
		if (!path)
		{
			g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
					ERROR_MESG_NO_FILE, ENU_CMD_NOT_FOUND});
			ft_free_tab(argv);
			exit (127);
		}
		if (execve(path, argv, envp) == -1)
		{
			remove_added_quotes(&cmd);
			g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
					ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
			if (path)
				free(path);
			ft_free_tab(argv);
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_minishell.env->exit_status = WEXITSTATUS(status);
		else
			g_minishell.env->exit_status = 0;
		ft_free_tab(argv);
	}
}

void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input)
{
	t_token	*curr;
	int pipe;
	
	if (!tk)
		return ;
	curr = tk;
	if (contain_char(input, '|'))
		pipe = 1;
	if (curr->type == T_PIPE || pipe == 1)
	{
		
		handle_pipe(tk, ft_env, *env, input);
	}
	else if (contain_list("<<", tk) || contain_list(">>", tk)
		|| contain_list("<", tk) || contain_list(">", tk))
		handle_redirection(tk, ft_env, *env);
	else
	{
		if (ft_is_builtin(curr->cmd))
			handle_builtin(tk, ft_env, env);
		else
			handle_path_command(tk, ft_env, curr->cmd);
	}
	return ;
}
