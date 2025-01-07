/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:43:13 by hal-moug          #+#    #+#             */
/*   Updated: 2024/06/21 08:43:14 by hal-moug         ###   ########.fr       */
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

static int	ft_count(char const *s, char c)
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

static char	**split(char **s_split, char const *s, char c, int count)
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
// int main() {
//     // Example string and delimiter
//     char const *s = "This is a sample string to split";
//     char delimiter = ' ';
//     // Call ft_split
//     char **s_split = ft_split(s, delimiter);
//     if (s_split) {
//         // If splitting was successful, print the individual substrings
//         printf("Splitting successful!\n");
//         for (int i = 0; s_split[i] != NULL; i++) {
//             printf("Substring %d: '%s'\n", i, s_split[i]);
//         }
//         // Free the dynamically allocated memory
//         ft_free(s_split);
//         printf("Memory freed successfully!\n");
//     } else {
//         // If ft_split returns NULL, memory allocation failed
//         printf("Memory allocation failed.\n");
//     }
//     return 0;
// }
