/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:25:34 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/16 10:35:10 by hal-moug         ###   ########.fr       */
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

static int	process_heredoc_line_input(char **line, char *delimiter)
{
	*line = readline("> ");
	if (!*line)
	{
		print_eof_warning(delimiter);
		return (0);
	}
	if (ft_strcmp(*line, delimiter) == 0)
	{
		free(*line);
		return (0);
	}
	return (1);
}

static int	check_heredoc_size(size_t total, size_t line_len,
			char *line, char *expanded)
{
	if (total + line_len + 1 > MAX_HEREDOC_SIZE)
	{
		ft_putendl_fd("minishell: heredoc: maximum size exceeded", 2);
		free(line);
		free(expanded);
		exit(1);
	}
	return (1);
}

void	heredoc_child_loop(int write_fd, char *delimiter,
		t_shell *shell, int quote)
{
	char	*line;
	char	*expanded_line;
	size_t	total_written;
	size_t	line_len;

	total_written = 0;
	while (1)
	{
		if (!process_heredoc_line_input(&line, delimiter))
			break ;
		expanded_line = expand_variables(line, shell->env, quote, &shell->gc);
		if (!expanded_line)
			expanded_line = ft_strdup(line);
		line_len = ft_strlen(expanded_line);
		check_heredoc_size(total_written, line_len, line, expanded_line);
		write(write_fd, expanded_line, line_len);
		write(write_fd, "\n", 1);
		total_written += line_len + 1;
		free(line);
		free(expanded_line);
	}
}
