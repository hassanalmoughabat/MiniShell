/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:23:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:23:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	**build_argv_from_tokens(t_token *tk)
{
	char	**argv;
	int		count;

	count = count_total_args(tk);
	if (count == 0)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	if (!fill_argv_from_tokens(argv, tk, count))
	{
		while (--count >= 0)
			free(argv[count]);
		free(argv);
		return (NULL);
	}
	return (argv);
}

void	free_shell_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	exit_error_msg(t_shell *shell, char *cmd, int code, char *msg)
{
	shell->env->exit_status = code;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(msg, 2);
	exit(code);
}

void	validate_executable(char *cmd, char *path, t_shell *shell)
{
	struct stat	sb;

	if (!stat(path, &sb))
	{
		if (S_ISDIR(sb.st_mode))
			exit_error_msg(shell, cmd, 126, " Is a directory");
		if (access(path, X_OK) != 0)
			exit_error_msg(shell, cmd, 126, ": Permission denied");
	}
	else
		exit_error_msg(shell, cmd, 127, " No such file or directory");
}
