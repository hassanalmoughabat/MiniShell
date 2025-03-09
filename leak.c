/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:50:35 by hal-moug          #+#    #+#             */
/*   Updated: 2025/03/09 12:53:37 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

void free_token_list(t_token *head)
{
    t_token *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->cmd);
        free(temp);
    }
}

void free_env_list(t_env *head)
{
    t_env *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }
}

void free_array(char **arr)
{
    int i = 0;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}