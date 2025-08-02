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

#include "ft_heredoc.h"

int	get_exit_code(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

char	*copy_without_quotes(char *str, int len)
{
	char	*result;
	int		i;
	int		j;

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

char	*extract_content_without_quotes(char *str, char quote_type)
{
	int	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if (str[0] == quote_type && str[len - 1] == quote_type)
		return (copy_without_quotes(str, len));
	return (ft_strdup(str));
}

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
