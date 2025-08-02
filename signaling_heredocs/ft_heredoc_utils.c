/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 02:30:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 02:30:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

char	*ft_realloc_buffer(char *old_buf, int new_size,
		char **dst, int result_len)
{
	char	*new_buf;

	new_buf = malloc(new_size);
	if (!new_buf)
	{
		free(old_buf);
		return (NULL);
	}
	ft_memcpy(new_buf, old_buf, result_len);
	free(old_buf);
	*dst = new_buf + result_len;
	return (new_buf);
}

int	ft_check_heredoc_size(size_t total_written, char *expanded_line,
		int *pipefd)
{
	size_t	line_len;

	line_len = ft_strlen(expanded_line);
	if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (0);
}

void	ft_write_heredoc_line(int fd, char *expanded_line,
		size_t *total_written)
{
	size_t	line_len;

	line_len = ft_strlen(expanded_line);
	write(fd, expanded_line, line_len);
	write(fd, "\n", 1);
	*total_written += line_len + 1;
}

char	*ft_init_expansion_buffer(char *line, char **src, char **dst)
{
	char	*result;

	result = malloc(1024);
	if (!result)
		return (NULL);
	*src = line;
	*dst = result;
	return (result);
}

int	ft_process_heredoc_line(char *line, t_heredoc_data *data)
{
	char	*expanded_line;

	if (!line || ft_strcmp(line, data->delimiter) == 0)
	{
		if (line)
			free(line);
		return (1);
	}
	expanded_line = expand_variables(line, data->shell->env, data->quote);
	if (!expanded_line)
		expanded_line = ft_strdup(line);
	if (ft_check_heredoc_size(*(data->total_written), expanded_line,
			data->pipefd) == -1)
	{
		free(line);
		free(expanded_line);
		return (-1);
	}
	ft_write_heredoc_line(data->pipefd[1], expanded_line, data->total_written);
	free(line);
	free(expanded_line);
	return (0);
}
