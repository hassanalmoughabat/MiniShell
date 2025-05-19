/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:42 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/16 14:32:44 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

// void	handle_pipe(t_token *lst, char **ft_env, t_env * env)
// {
// 	ft_printf("hey");
// }

int	ft_is_builtin(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if ((ft_strcmp(curr->cmd, "cd") == 0))
			return (1);
		else if (ft_strcmp(curr->cmd, "echo") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "unset") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			return (1);
		else if (ft_strcmp(curr->cmd, "env") == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	is_a_path_command(char *cmd, char **ft_env)
{
	char	*path_env;
	int		i;
	char	*start;
	char	*colon;
	char	*dir;
	char	*tmp;
	char	*full_path;
	int		found;
	size_t	len;

	path_env = NULL;
	i = 0;
	while (ft_env[i])
	{
		if (ft_strncmp(ft_env[i], "PATH=", 5) == 0)
		{
			path_env = ft_env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
	{
		ft_putstr_fd("Error: PATH environment variable not found\n", 2);
		return (0);
	}
	found = 0;
	start = path_env;
	while (*start)
	{
		colon = ft_strchr(start, ':');
		if (colon)
			len = (size_t)(colon - start);
		else
			len = ft_strlen(start);
		dir = ft_substr(start, 0, len);
		if (!dir)
			return (0);
		tmp = ft_strjoin(dir, "/");
		free(dir);
		if (!tmp)
			return (0);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			found = 1;
			free(full_path);
			break ;
		}
		else
			g_minishell.env->exit_status = ft_err_msg((t_error){cmd, ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
		free(full_path);
		if (colon)
			start = colon + 1;
		else
			break ;
	}
	if (!found)
		g_minishell.env->exit_status = ft_err_msg((t_error){cmd, ERROR_MESG_CMD_NOT_FOUND, ENU_CMD_NOT_FOUND});
	return (0);
}
