/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkhrayza <hkhrayza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:46:14 by hkhrayza          #+#    #+#             */
/*   Updated: 2024/06/15 16:59:23 by hkhrayza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *ptr)
{
	int	result;
	int	i;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (ptr[i] != '\0' && ((ptr[i] > 8 && ptr[i] < 14) || ptr[i] == 32))
		i++;
	if (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			sign = -1;
		i++;
	}
	while (ptr[i] != '\0' && ptr[i] >= '0' && ptr[i] <= '9')
	{
		result = result * 10 + ptr[i] - '0';
		i++;
	}
	return (sign * result);
}
