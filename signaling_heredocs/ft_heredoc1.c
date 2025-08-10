/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:56:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:11:01 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

char	*expand_variables(char *line, t_env *env, int quote)
{
	char				*result;
	char				*src;
	char				*dst;

	if (quote == 1 || !line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	result = ft_init_expansion_buffer(line, &src, &dst);
	if (!result)
		return (NULL);
	return (ft_setup_and_process_expansion(src, env, dst, result));
}

int	handle_dless(char *delimiter, t_shell *shell, int quote)
{
	int				pipefd[2];
	size_t			total_written;
	char			*line;
	int				result;
	t_heredoc_data	data;

	if (ft_setup_heredoc_pipe(delimiter, pipefd) == -1)
		return (-1);
	total_written = 0;
	data.delimiter = delimiter;
	data.shell = shell;
	data.quote = quote;
	data.pipefd = pipefd;
	data.total_written = &total_written;
	while (1)
	{
		line = readline("> ");
		result = ft_process_heredoc_line(line, &data);
		if (result == 1)
			break ;
		if (result == -1)
			return (-1);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_check_quotes(str[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	ft_process_heredoc_token(t_shell *shell, t_token *curr)
{
	char				*delimiter;
	char				*cmd;
	int					quote;
	t_heredoc_params	params;

	params.curr = curr;
	params.delimiter = &delimiter;
	params.cmd = &cmd;
	params.quote = &quote;
	params.shell = shell;
	if (ft_setup_heredoc_params(&params) == -1)
		return (-1);
	return (ft_fork_and_execute_heredoc(shell, delimiter, cmd, quote));
}

void	handle_heredoc(t_shell *shell)
{
	t_token	*curr;
	int		result;

	curr = shell->tk;
	while (curr)
	{
		if (curr->cmd && ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
		{
			result = ft_process_heredoc_token(shell, curr);
			if (result == -1)
				return ;
			if (result == 1)
				return ;
			curr = curr->next->next;
			continue ;
		}
		curr = curr->next;
	}
}
