/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:06:46 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/30 16:06:48 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex(unsigned long nbr, char c)
{
	int	len;

	len = 0;
	if (nbr > 15)
		len += ft_puthex(nbr / 16, c);
	if (c == 'x')
		len += write(1, &"0123456789abcdef"[nbr % 16], 1);
	else
		len += write(1, &"0123456789ABCDEF"[nbr % 16], 1);
	return (len);
}
