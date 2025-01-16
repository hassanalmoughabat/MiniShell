/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:06:38 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/30 16:06:40 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(int nb)
{
	long	nbr;
	int		len;

	len = 0;
	nbr = nb;
	if (nbr < 0)
	{
		len += ft_putchar('-');
		nbr = nbr * -1;
	}
	if (nbr > 9)
		len += ft_putnbr(nbr / 10);
	len += ft_putchar(nbr % 10 + '0');
	return (len);
}
