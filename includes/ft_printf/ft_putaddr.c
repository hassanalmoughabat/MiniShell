/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:07:09 by hal-moug          #+#    #+#             */
/*   Updated: 2025/02/17 12:55:32 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	ft_strlen(char const *str)
{
	int	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

int	ft_putaddr(size_t addr)
{
	int	len;

	if (!addr)
	{
		ft_putstr("(nil)");
		return (ft_strlen("(nil)"));
	}
	len = 0;
	len += ft_putstr("0x");
	len += ft_puthex(addr, 'x');
	return (len);
}
