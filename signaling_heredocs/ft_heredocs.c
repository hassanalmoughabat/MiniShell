/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:56:14 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/06 18:59:30 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	get_exit_code(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

char	*extract_content_without_quotes(char *str, char quote_type)
{
	int		i;
	int		j;
	int		len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if (str[0] == quote_type && str[len - 1] == quote_type)
	{
		result = (char *)malloc(sizeof(char) * (len - 1));
		if (!result)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
			result[j++] = str[i++];
		result[j] = '\0';
		return (result);
	}
	return (ft_strdup(str));
}

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*get_delimeter(t_token *tk)
{
	t_token	*curr;
	char	*delimeter;
	int		len;

	curr = tk;
	while (curr)
	{
		if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
		{
			delimeter = curr->next->cmd;
			len = ft_strlen(delimeter);
			if (len >= 2 && delimeter[0] == '\'' && delimeter[len - 1] == '\'')
				return (ft_strsub(delimeter, 1, len - 2));
			else if (len >= 2 && delimeter[0] == '\"'
				&& delimeter[len - 1] == '\"')
				return (ft_strsub(delimeter, 1, len - 2));
			else
				return (ft_strdup(delimeter));
		}
		curr = curr->next;
	}
	return (NULL);
}

char	*find_command_around_heredoc(t_token *tk, char *delimiter)
{
	t_token	*curr;
	t_token	*command_before;

	curr = tk;
	command_before = NULL;
	while (curr)
	{
		if (curr->next && ft_strcmp(curr->next->cmd, "<<") == 0)
			command_before = curr;
		if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
		{
			curr = curr->next;
			while (curr)
			{
				if (ft_strcmp(curr->cmd, delimiter) == 0 && curr->next)
					return (curr->next->cmd);
				curr = curr->next;
			}
			if (command_before)
				return (command_before->cmd);
			return (NULL);
		}
		curr = curr->next;
	}
	return (NULL);
}
