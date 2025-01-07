/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:42:43 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/21 08:42:46 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int x, size_t n)
{
	size_t			i;
	unsigned char	*char_ptr;

	char_ptr = ptr;
	i = 0;
	while (i < n)
	{
		char_ptr[i] = (unsigned char)x;
		i++;
	}
	return (ptr);
}
