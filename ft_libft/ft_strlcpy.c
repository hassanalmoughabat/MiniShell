/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkhrayza <hkhrayza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:58:01 by hkhrayza          #+#    #+#             */
/*   Updated: 2024/06/15 16:59:23 by hkhrayza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (src[i] != '\0')
		i++;
	if (!size)
		return (i);
	j = 0;
	while (src[j] != '\0' && j < size -1)
	{
		dest[j] = src[j];
		j++;
	}
	dest[j] = '\0';
	return (i);
}
