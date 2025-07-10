/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:20:15 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/04 22:01:47 by njoudieh42       ###   ########.fr       */
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
			if ((tk[i] == '/'))
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


void free_shell_args(char **args)
{
    int i = 0;
    if (!args) return;
    while (args[i]) free(args[i++]);
    free(args);
}

void print_double_string(char **arr)
{
    int i = 0;
    if (!arr)
    {
        printf("(null)\n");
        return;
    }
    while (arr[i])
    {
        printf("[%d]: %s\n", i, arr[i]);
        i++;
    }
}

void	handle_path_command(t_token *tk, char *envp[], char *cmd)
{
	char	**argv;
	char	*path;
	pid_t	pid;
	int		status;
	struct stat sb;

	if (!cmd || !tk)
		return ;
	if (!ft_strcmp(tk->cmd, "\"\"") || !ft_strcmp(tk->cmd, "''"))
	{
		g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
				ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
		return ;
	}
	argv = build_argv_from_tokens(tk);
	if (!argv || !argv[0] || !argv[0][0] )
	{
		if (argv)
			free_shell_args(argv);
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
		signal(SIGQUIT, SIG_DFL);
		path = get_path(argv[0], envp);
		if (!path)
		{
			g_minishell.env->exit_status = ft_err_msg((t_error){(cmd),
					ERROR_MESG_NO_FILE, ENU_CMD_NOT_FOUND});
			ft_free_tab(argv);
			exit (127);
		}
		if (contains_symbols(cmd, 1))
		{
			if(stat(path, &sb) == 0)
			{
				if (S_ISDIR(sb.st_mode))
				{
					g_minishell.env->exit_status = 126;
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(cmd, 2);
					ft_putstr_fd(" Is a directory \n", 2);
					exit(126);
				}
				else if (access(path, X_OK) != 0) {
					g_minishell.env->exit_status = 126;
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(cmd, 2);
					ft_putstr_fd(": Permission denied\n", 2);
					exit(126);
				}
			}
			else
			{
				g_minishell.env->exit_status = 127;
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(" No such file or directory \n", 2);
				exit (127);
			}
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
		if (WIFSIGNALED(status))
			g_minishell.env->exit_status = 128 + WTERMSIG(status);
		else if (WIFEXITED(status))
			g_minishell.env->exit_status = WEXITSTATUS(status);
		else
			g_minishell.env->exit_status = 0;
		ft_free_tab(argv);
	}
}

void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input)
{
	t_token	*curr;
	int		pipe;
	int		status;
	pid_t	pid;

	if (!tk)
		return ;
	curr = tk;
	if (!ft_strcmp(tk->cmd, ":") || !ft_strcmp(tk->cmd, "!"))
		return ;
	if (!ft_strncmp(tk->cmd, ";",1))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		if(tk->cmd[1] == ';')
			ft_putstr_fd(";;'", 2);
		else
			ft_putstr_fd(";'", 2);
		ft_putstr_fd("\n",2);
		(*env)->exit_status = 2;
		return ;
	}
	if (contain_list("|", tk))
		handle_pipe(tk, ft_env, *env, input);
	else if (contain_list("<<", tk) || contain_list(">>", tk)
		|| contain_list("<", tk) || contain_list(">", tk))
	{
		pid = fork();
		if (pid == 0)
		{
			handle_redirection(tk, ft_env, *env, input);
			exit((*env)->exit_status);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			if (WIFSIGNALED(status))
				(*env)->exit_status = 128 + WTERMSIG(status);
			else if (WIFEXITED(status))
				(*env)->exit_status = WEXITSTATUS(status);
		}
	}
	else
	{
		if (ft_is_builtin(curr->cmd))
			handle_builtin(tk, ft_env, env);
		else
			handle_path_command(tk, ft_env, curr->cmd);
	}
	return ;
}
