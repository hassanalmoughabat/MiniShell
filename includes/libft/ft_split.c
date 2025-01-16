/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 22:12:43 by njoudieh42        #+#    #+#             */
/*   Updated: 2024/12/19 22:16:26 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
		free(arr);
	}
}

int	ft_count(char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
		else if (s[i] == c)
			i++;
	}
	return (count);
}

int	word_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	return (i);
}

char	**split(char **s_split, char const *s, char c, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (s[j] == c && s[j] != '\0')
			j++;
		s_split[i] = ft_substr(s, j, word_len(&*(s + j), c));
		if (!s_split[i])
		{
			ft_free (s_split);
			return (NULL);
		}
		while (s[j] != '\0' && s[j] != c)
			j++;
		i++;
	}
	s_split[count] = NULL;
	return (s_split);
}

char	**ft_split(char const *s, char c)
{
	char	**s_split;
	int		count;

	if (!s)
		return (NULL);
	count = ft_count(s, c);
	s_split = (char **)malloc((count + 1) * sizeof(char *));
	if (!s_split)
		return (NULL);
	s_split = split(s_split, s, c, count);
	return (s_split);
}
