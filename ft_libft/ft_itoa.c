/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkhrayza <hkhrayza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:47:08 by hkhrayza          #+#    #+#             */
/*   Updated: 2024/06/15 16:59:23 by hkhrayza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lennb(int nb, int *sign)
{
	int	len_numb;

	len_numb = 0;
	if (!nb)
		return (0);
	if (nb == -2147483648)
	{
		*sign = -1;
		len_numb = 2;
		nb = 147483648;
	}
	if (nb < 0)
	{
		*sign = -1;
		len_numb++;
		nb = -nb;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		len_numb ++;
	}
	return (len_numb);
}

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

char	*ft_insert(char *ptr, int len_numb, int n)
{
	n = ft_abs(n);
	if (n == 0)
		ptr[0] = '0';
	while (n)
	{
		ptr[len_numb] = n % 10 + '0';
		n = n / 10;
		len_numb--;
	}
	return (ptr);
}

char	*ft_itoa(int n)
{
	char	*numb;
	int		sign;
	int		len_numb;

	len_numb = 1;
	sign = 1;
	len_numb = len_numb + ft_lennb(n, &sign);
	numb = (char *)malloc((len_numb + 1) * sizeof(char));
	if (!numb)
		return (NULL);
	numb[len_numb] = '\0';
	len_numb--;
	if (n == -2147483648)
	{
		numb[1] = '2';
		n = 147483648;
	}
	numb = ft_insert(numb, len_numb, n);
	if (sign == -1)
		numb[0] = '-';
	return (numb);
}
