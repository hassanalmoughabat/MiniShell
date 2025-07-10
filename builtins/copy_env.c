/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 23:34:20 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/06/30 17:52:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_free_env(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->line);
		free(tmp);
	}
}

t_env	*new_env_node(char *key_value)
{
	t_env	*node;

	if (!key_value)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->line = ft_strdup(key_value);
	if (!node->line)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_env	*copy_env(t_env *envp)
{
	t_env	*head;
	char	*key;
	char	*value;
	char	*temp;
	char	*equal_ptr;
	t_env	*curr1;
	t_env	*new_node;
	t_env	*current;

	head = NULL;
	current = envp;
	while (current)
	{
		equal_ptr = ft_strchr(current->line, '=');
		key = ft_substr(current->line, 0, equal_ptr - current->line);
		equal_ptr ++;
		if (equal_ptr)
			value = ft_strdup(equal_ptr);
		temp = quotes_in_env(value, key, 0);
		if (!temp)
			return (ft_free_env(head), NULL);
		new_node = new_env_node(temp);
		if (!new_node)
			return (ft_free_env(head), NULL);
		if (!head)
		{
			head = new_node;
			curr1 = new_node;
		}
		else
		{
			curr1->next = new_node;
			curr1 = curr1->next;
		}
		current = current->next;
	}
	return (head);
}

void	print_env_list(t_env *env_list)
{
	while (env_list)
	{
		ft_printf("%s\n", env_list->line);
		env_list = env_list->next;
	}
}
