/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:06:24 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/30 16:06:27 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunbr(unsigned int nbr)
{
	int	len;

	len = 0;
	if (nbr > 9)
		len += ft_putunbr(nbr / 10);
	len += ft_putchar(nbr % 10 + '0');
	return (len);
}
