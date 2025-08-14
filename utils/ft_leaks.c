/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:18:18 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/06 17:19:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	init_gc(t_gc *gc)
{
	if (!gc)
		return ;
	ft_memset(gc, 0, sizeof(t_gc));
}

void	*ft_malloc(t_gc *gc, size_t size)
{
	t_mem_node	*new_node;
	void		*ptr;

	if (!gc)
		return (malloc(size));
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, size);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = gc->head;
	gc->head = new_node;
	return (ptr);
}

char	*ft_strdup_gc(t_gc *gc, const char *s)
{
	char	*d;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
		i++;
	i++;
	d = ft_malloc(gc, i * sizeof(char));
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


char	*ft_strjoin_gc(const char *s1, const char *s2, t_gc *gc)
{
	char	*dest;
	size_t	pref_len;
	size_t	suf_len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	pref_len = ft_strlen(s1);
	suf_len = ft_strlen(s2);
	dest = (char *)ft_malloc(gc, (pref_len + suf_len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < pref_len)
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < suf_len)
		dest[i++] = s2[j++];
	dest[i] = '\0';
	return (dest);
}

void	ft_free_all(t_gc *gc)
{
	t_mem_node	*curr;
	t_mem_node	*tmp;

	if (!gc || !gc->head)
		return ;
	curr = gc->head;
	gc->head = NULL;
	while (curr)
	{
		tmp = curr;
		curr = curr->next;
		if (tmp->ptr)
			free(tmp->ptr);
		free(tmp);
	}
}
