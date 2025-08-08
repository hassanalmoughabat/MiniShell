/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 01:10:25 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 01:11:05 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"
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

int	ft_setup_heredoc_params(t_heredoc_params *params)
{
	char	*original_delimiter;

	*(params->delimiter) = get_delimeter(params->curr);
	if (!*(params->delimiter))
		return (-1);
	original_delimiter = ft_strdup(*(params->delimiter));
	*(params->quote) = has_quotes(*(params->delimiter));
	remove_added_quotes(params->delimiter);
	*(params->cmd) = find_command_around_heredoc(params->shell->tk,
			original_delimiter);
	free(original_delimiter);
	return (0);
}
