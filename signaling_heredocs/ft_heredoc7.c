/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:25:34 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/13 12:00:33 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	print_eof_warning(char *delimiter)
{
	ft_putstr_fd("bash: warning: here-document delimited by ", 2);
	ft_putstr_fd("end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
}

int	write_line_to_pipe(t_heredoc_data *data, char *expanded_line)
{
	size_t	line_len;

	line_len = ft_strlen(expanded_line);
	if (data->total_written + line_len + 1 > MAX_HEREDOC_SIZE)
	{
		ft_putendl_fd("minishell: heredoc: maximum size exceeded", 2);
		close(data->pipefd[0]);
		close(data->pipefd[1]);
		ft_init_signals();
		return (0);
	}
	write(data->pipefd[1], expanded_line, line_len);
	write(data->pipefd[1], "\n", 1);
	data->total_written += line_len + 1;
	return (1);
}

int	process_heredoc_line(char *line, t_heredoc_data *data,
		t_shell *shell)
{
	char	*expanded_line;
	int		result;

	if (ft_strcmp(line, data->delimiter) == 0)
	{
		free(line);
		return (2);
	}
	expanded_line = expand_variables(line, shell->env, data->quote, &shell->gc);
	if (!expanded_line)
		expanded_line = ft_strdup_gc(&shell->gc, line);
	result = write_line_to_pipe(data, expanded_line);
	if (!result)
		return (0);
	return (1);
}

int	heredoc_loop(t_heredoc_data *data, t_shell *shell)
{
	char	*line;
	int		process_result;

	while (1)
	{
		line = readline("> ");
		if (g_signal.heredoc_sigint)
		{
			close(data->pipefd[0]);
			close(data->pipefd[1]);
			ft_init_signals();
			return (shell->env->exit_status = 130, -1);
		}
		if (!line)
		{
			print_eof_warning(data->delimiter);
			break ;
		}
		process_result = process_heredoc_line(line, data, shell);
		if (process_result == 2)
			break ;
		if (process_result == 0)
			return (0);
	}
	return (1);
}

int	handle_dless(char *delimiter, t_shell *shell, int quote)
{
	t_heredoc_data	data;
	int				result;

	if (!validate_delimiter(delimiter) || pipe(data.pipefd) == -1)
		return (-1);
	data.total_written = 0;
	data.delimiter = delimiter;
	data.quote = quote;
	ft_set_heredoc_signals();
	g_signal.heredoc_sigint = false;
	result = heredoc_loop(&data, shell);
	if (result == -1)
		return (ft_init_signals(), -1);
	if (!result)
	{
		close(data.pipefd[0]);
		close(data.pipefd[1]);
		ft_init_signals();
		return (-1);
	}
	close(data.pipefd[1]);
	ft_init_signals();
	return (data.pipefd[0]);
}
