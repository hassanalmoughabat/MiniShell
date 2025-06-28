/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_great_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/28 16:38:10 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int		count_tokens(t_token *tokens)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = tokens;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

char	**build_args_array(t_token *cmd_tokens)
{
	char	**args;
	t_token	*curr;
	int		count;
	int		i;

	count = count_tokens(cmd_tokens);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	curr = cmd_tokens;
	while (curr)
	{
		args[i] = ft_strdup(curr->cmd);
		if (!args[i])
		{
			ft_free_tab(args);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

void	print_cmd_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

void	execute_external_cmd(t_token *cmd_tokens, char **ft_env)
{
	char	**args;
	char	*path;

	args = build_args_array(cmd_tokens);
	if (!args)
		exit(EXIT_FAILURE);
	path = get_path(args[0], ft_env);
	if (!path)
	{
		print_cmd_error(args[0], "command not found");
		ft_free_tab(args);
		free_token_list(cmd_tokens);
		exit(EXIT_FAILURE);
	}
	free_token_list(cmd_tokens);
	if (execve(path, args, ft_env) == -1)
	{
		printf("i am leaking  and getting out\n");
		// print_cmd_error(args[0], strerror(errno));
		ft_free_tab(args);
		free(path);
		free_token_list(cmd_tokens);
		exit(EXIT_FAILURE);
	}
}

void	execute_with_redirect(t_token *cmd_tokens, char **ft_env,
		t_env *env, int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	if (ft_is_builtin(cmd_tokens->cmd))
		execute_builtin_redirect(cmd_tokens, ft_env, env);
	else
		execute_external_cmd(cmd_tokens, ft_env);
}
