/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 22:12:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2024/12/19 22:12:07 by njoudieh42       ###   ########.fr       */
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
