/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 16:09:27 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	has_equal(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	equal_handler_export(char *input, char **key, char **value, int flag, t_env *env)
{
	char	*temp;

	if (flag == 1)
	{
		temp = get_key(input);
		if (!temp)
		{
			ft_printf("Error in key syntax \n");
			return (-1);
		}
		*key = temp;
		*value = get_value(input,env);
		if (remove_added_quotes(key, 1) == -1 || ( value && remove_added_quotes(value, 0) == -1))
		{
			ft_printf("Error in the variable syntax or the value syntax\n");
			return (-1);
		}
	}
	if (flag == 2)
	{
		*key = NULL;
		*value = NULL;
		return (0);
	}
	return (1);
}

void swap_env_nodes(t_env *a, t_env *b)
{
    char *temp = a->line;
    a->line = b->line;
    b->line = temp;
}

void	sort_env_list(t_env *env)
{
	int swapped;
	t_env *ptr;
	t_env *last;

	last = NULL;
	if (!env || !env)
		return ;
	while (swapped)
	{
		swapped = 0;
		ptr = env;
		while (ptr->next != last)
		{
			if (strcmp(ptr->line, ptr->next->line) > 0)
			{
				swap_env_nodes(ptr, ptr->next);
				swapped = 1;
			}
			ptr = ptr->next;
		}
		last = ptr;
	}
}

void	print_export_env( t_env *env)
{
	if (ft_list_size(token) == 1 && curr->cmd && !ft_strcmp(curr->cmd, "export"))
    while (env)
    {
        printf("declare -x %s\n",env->line);
        env = env->next;
    }
}

void	respective_addition(t_env **env, t_env **copy, char *key, char *value, int flag)
{
	if (flag == 1)
	{
		if (check_if_var_exist(env, key))
			ft_update_env(key, value, env, copy);
		else
			ft_add_env(key, value, env, copy);
		return ;
	}
	if (flag == 2)
	{
		if (check_if_var_exist(env, key))
			ft_update_env(key, get_value_from_env(key, *env), env, copy);
		else
			ft_add_key_to_env(env, copy, key);
		return ;
	}
}

void	ft_export(t_token *token, t_env *env)
{
	t_token	*curr;
	t_env	*copy;
	char	*key;
	char	*value;
	int		i;

	copy = copy_env(env);
	curr = token;
	key = NULL;
	value = NULL;
	if (print_export_env(env))
			return ;
	curr = curr->next;
	while (curr && curr->cmd)
	{
		
	
		i = set_key_value(curr->cmd, &key, &value,env);
		if (i == 1)
		{
			if (has_equal(curr->cmd))
				respective_addition(&env, &copy, key, value, 1);
			else
				respective_addition(&env, &copy, key, value, 2);
		}
		if (i == 2)
			return ;
		curr = curr->next;
	}
}
