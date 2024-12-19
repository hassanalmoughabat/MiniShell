/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkhrayza <hkhrayza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:57:51 by hkhrayza          #+#    #+#             */
/*   Updated: 2024/06/15 16:59:23 by hkhrayza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	int		pref_len;
	int		suf_len;
	int		i;

	pref_len = 0;
	suf_len = 0;
	i = -1;
	while (s1[pref_len] != '\0')
		pref_len++;
	while (s2[suf_len] != '\0')
		suf_len++;
	dest = (char *)malloc((pref_len + suf_len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (s1[++i] != '\0')
		dest[i] = s1 [i];
	i = -1;
	while (s2[++i] != '\0')
		dest[pref_len + i] = s2[i];
	dest[pref_len + i] = '\0';
	return (dest);
}
