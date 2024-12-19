/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkhrayza <hkhrayza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:58:17 by hkhrayza          #+#    #+#             */
/*   Updated: 2024/06/15 16:59:23 by hkhrayza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*d;
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	i++;
	d = (char *)malloc(i * sizeof(char));
	if (!d)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		d[i] = (*f)(i, s[i]);
		i++;
	}
	d[i] = '\0';
	return (d);
}
