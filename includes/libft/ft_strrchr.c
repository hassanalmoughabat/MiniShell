/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:44:11 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/21 08:44:13 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	uc;
	char			*tmp;

	tmp = NULL;
	uc = (unsigned char)c;
	while (*s != '\0')
	{
		if (*s == uc)
			tmp = (char *)s;
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return (tmp);
}
