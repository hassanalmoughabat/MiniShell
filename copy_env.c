/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 23:34:20 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 00:32:10 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

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

t_env *new_env_node(char *key_value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->line = strdup(key_value);
    if (!node)
    {
        free(node);
        return (NULL);
    }
    node->next = NULL;
    return (node);
}

t_env *copy_env(t_env *envp)
{
    t_env	*head;
    t_env   *curr1;
	t_env	*new_node;
    t_env	*current;

	head = NULL;
	current = envp;
	while (current)
    {
        new_node = new_env_node(current->line);
        if (!new_node)
            return (ft_free_env(head),NULL);
        if (!head)
        {
            head = new_node;
            curr1= new_node;
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

void print_env_list(t_env *env_list)
{
    while (env_list)
    {
        printf("%s\n", env_list->line);
        env_list = env_list->next;
    }
}
