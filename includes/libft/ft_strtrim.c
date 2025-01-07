/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:44:19 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/21 08:44:20 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_char(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim_s1;
	int		start;
	int		end;
	int		i;

	start = 0;
	i = -1;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	while (end > 0 && is_char(s1[end - 1], set))
		end--;
	while (s1[start] && is_char(s1[start], set))
		start++;
	if (start >= end)
		return (ft_strdup(""));
	trim_s1 = (char *)malloc((end - start + 1) * sizeof(char));
	if (!trim_s1)
		return (NULL);
	while (++i < end - start)
		trim_s1[i] = s1[start + i];
	trim_s1[end - start] = '\0';
	return (trim_s1);
}
