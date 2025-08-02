/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 02:35:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 02:35:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

static int	ft_process_dollar_expansion(t_expansion_params *params)
{
	t_expansion_params	dollar_params;

	dollar_params.src = params->src;
	dollar_params.env = params->env;
	dollar_params.dst = params->dst;
	dollar_params.result_len = params->result_len;
	dollar_params.buffer_size = params->buffer_size;
	dollar_params.result = params->result;
	return (ft_handle_dollar_expansion(&dollar_params));
}

static int	ft_process_regular_char(t_expansion_params *params)
{
	t_char_expansion_params	char_params;

	char_params.src = params->src;
	char_params.dst = &(params->dst);
	char_params.result_len = params->result_len;
	char_params.buffer_size = params->buffer_size;
	char_params.result = params->result;
	return (ft_handle_regular_char_expansion(&char_params));
}

char	*ft_process_expansion_loop(t_expansion_params *params)
{
	while (*(params->src))
	{
		if (**(params->src) == '$'
			&& (ft_isalpha(*(params->src)[1])
			|| *(params->src)[1] == '_'))
		{
			if (!ft_process_dollar_expansion(params))
				return (NULL);
		}
		else
		{
			if (!ft_process_regular_char(params))
				return (NULL);
		}
	}
	return (*(params->dst) = '\0',*(params->result));
}

int	ft_setup_heredoc_params(t_heredoc_params *params)
{
	*(params->delimiter) = get_delimeter(params->curr);
	if (!*(params->delimiter))
		return (-1);
	*(params->quote) = has_quotes(*(params->delimiter));
	remove_added_quotes(params->delimiter);
	*(params->cmd) = find_command_around_heredoc(params->shell->tk,
			*(params->delimiter));
	return (0);
}

int	ft_setup_heredoc_pipe(char *delimiter, int *pipefd)
{
	signal(SIGINT, ft_heredoc_sigint_handler);
	if (!validate_delimiter(delimiter) || pipe(pipefd) == -1)
		return (-1);
	return (0);
}

int	ft_fork_and_execute_heredoc(t_shell *shell, char *delimiter,
		char *cmd, int quote)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		free(delimiter);
		return (-1);
	}
	if (pid == 0)
		ft_handle_heredoc_child(shell, delimiter, cmd, quote);
	else
	{
		free(delimiter);
		ft_handle_heredoc_parent(shell, pid, status);
		if (g_signal.heredoc_sigint)
			return (1);
	}
	return (0);
}

char	*ft_setup_and_process_expansion(char *src,
		t_env *env, char *dst, char *result)
{
	int					result_len;
	int					buffer_size;
	t_expansion_params	params;

	result_len = 0;
	buffer_size = 1024;
	params.src = &src;
	params.env = env;
	params.dst = dst;
	params.result_len = &result_len;
	params.buffer_size = &buffer_size;
	params.result = &result;
	return (ft_process_expansion_loop(&params));
}
