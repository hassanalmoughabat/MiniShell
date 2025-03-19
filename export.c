/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/19 13:23:58 by njoudieh42       ###   ########.fr       */
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

int	equal_handler_export(char *input, char **key, char **value, int flag)
{
	char	*temp;

	if (flag == 1)
	{
		temp = get_key(input);
		// ft_printf(" INSIDE EQUAL_HANDLER_EXPORT FN\n");
		// ft_printf("%s\n",temp);
		if (!temp)
		{
			ft_printf("Error in allocating the key\n");
			return (-1);
		}
		*key = temp;
			*value = get_value(input);
		// ft_printf("whatttt\n");
		if (remove_added_quotes(key, 1) == -1 || ( value && remove_added_quotes(value, 0) == -1))
		{
			ft_printf("Error in the variable syntax or the value syntax\n");
			return (-1);
		}
	}
	if (flag == 3)
	{
		if (remove_added_quotes(key, 1) == -1 )
		{
			ft_printf("Error in the variable syntax or the value syntax\n");
			return (-1);
		}
		else
			ft_printf("%s\n",key);
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

// void	add_declare_x(t_env *env)
// {
// 	t_env	*current;
// 	char	*new;

// 	if (!env || !env)
// 		return ;
// 	current = env;
// 	while (current)
// 	{
// 		if (!current->line || ft_strlen(current->line) < 11
// 			|| ft_strncmp(current->line, "declare -x ", 11))
// 		{
// 			new= ft_strjoin("declare -x ",current->line);
// 			if (!new)
// 				return ;
// 			free(current->line);
// 			current->line = new;
// 		}
// 		current = current->next;
// 	}
// }

int	print_export_env(t_token *token, t_env *env)
{
	t_token	*current;
	t_env	*e;
	int		len;

	len = ft_list_size(token);
	e = env;
	current = token;
	if (len == 1 && current->cmd && !ft_strcmp(current->cmd, "export"))
	{
		sort_env_list(env);
		while (e)
		{
			ft_printf("declare -x %s\n",e->line);
			e = e->next;
		}
		return (1);
	}
	return (0);
}

void	respective_addition(t_env **env, char *key, char *value, int flag)
{
	if (flag == 1)
	{
		if (check_if_var_exist(env, key))
			ft_update_env(key, value, env);
		else
			ft_add_env(key, value, env);
		return ;
	}
	if (flag == 2)
	{
		if (check_if_var_exist(env, key))
			ft_update_env(key, get_value_from_env(key, *env), env);
		else
			ft_add_key_to_env(env, key);
		return ;
	}
}

void	ft_export(t_token *token, t_env *env)
{
	t_token	*curr;
	char	*key;
	char	*value;
	int		i;

	curr = token;
	key = NULL;
	value = NULL;
	if (print_export_env(token, env))
		return ;
	curr = curr->next;
	while (curr && curr->cmd)
	{
		i = set_key_value(curr->cmd, &key, &value,env);
		if (i == 1)
		{
			// ft_printf("OHHH NOOO\n");
			// ft_printf("%s %s",key,value);
			if (has_equal(curr->cmd))
				respective_addition(&env, key, value, 1);
			else if (check_if_var_exist(&env, key) && value)
			{
				// ft_printf("HELPP\n");
				ft_update_env(key, get_value_from_env(key, env), &env);
				// ft_printf("%s %s",key,get_value_from_env(key, env));
				// ft_printf("YEEESHHH\n");
			}
			else
				respective_addition(&env, key, value, 2);
		}
		else if (i == 2)
			return ;
		curr = curr->next;
	}
}
