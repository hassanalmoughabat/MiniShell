#include "includes/minihell.h"

char	*ft_strdup(const char *s)
{
	char	*d;
	int		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	i++;
	d = (char *)malloc((i * sizeof(char)));
	if (!d)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

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

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;
	size_t	remain_len;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\0')
		i++;
	if (start >= i)
		return (ft_strdup(""));
	remain_len = i - start;
	if (len > remain_len)
		len = remain_len;
	dest = (char *)malloc((len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0' && i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	int		pref_len;
	int		suf_len;
	int		i;

	pref_len = 0;
	suf_len = 0;
	i = -1;
	while (s1[pref_len] != '\0')
		pref_len++;
	while (s2[suf_len] != '\0')
		suf_len++;
	dest = (char *)malloc((pref_len + suf_len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (s1[++i] != '\0')
		dest[i] = s1 [i];
	i = -1;
	while (s2[++i] != '\0')
		dest[pref_len + i] = s2[i];
	dest[pref_len + i] = '\0';
	return (dest);
}
