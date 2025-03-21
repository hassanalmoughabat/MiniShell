/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:40:11 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/19 22:44:46 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

void delete_env_node(t_env **env, char *target)
{
    t_env *current;
    t_env *prev;

    if (!env || !*env || !target)
        return;
    
    current = *env;
    prev = NULL;

    while (current)
    {
        if (current->line && !ft_strncmp(current->line, target, ft_strlen(target)))
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            
            free(current->line);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void	ft_unset(t_token *token, t_env **env)
{
	t_token	*curr;
	t_env	*current;
	char	*key;

	if (!token || !(*env) || !env)
		return ;
	curr = token;
	curr = curr->next;
	
	while (curr)
	{
		key = get_key(curr->cmd);
		current = *env;
		while (current)
		{
			if (extract_key_env(current->line) == ft_strlen(key) && !ft_strncmp(current->line, key, ft_strlen(key)))
			{
				delete_env_node(env, key);
				break ;
			}
			current = current->next;
		}
		curr = curr->next;
	}
	return ;
}
