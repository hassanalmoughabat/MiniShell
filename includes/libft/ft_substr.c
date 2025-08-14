/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 22:14:21 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/16 15:51:40 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len, struct s_garbage_collector *gc)
{
	char	*dest;
	size_t	i;
	size_t	remain_len;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\0')
		i++;
	if (start >= i)
		return (ft_strdup(""));
	remain_len = i - start;
	if (len > remain_len)
		len = remain_len;
	dest = (char *)ft_malloc(gc, (len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0' && i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
