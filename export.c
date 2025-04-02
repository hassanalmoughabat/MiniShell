/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:08:41 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/01 21:06:44 by njoudieh42       ###   ########.fr       */
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
	char	quote;
	int		indicator;

	indicator = 0;
	quote ='\0';
	if (flag == 1)
	{
		temp = get_key(input, env, &quote, &indicator);
		ft_printf("the key after get_key %s\n",temp);
		if (!temp)
		{
			ft_printf("Error in key syntax \n");
			return (-1);
		}
		*key = temp;
		*value = get_value(input, env,quote, indicator);
		ft_printf("%s\n",*value);
		if (!*value)
			*value = ft_strdup("\"\"");
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

int	print_export_env(  t_token *token,t_env *env)
{
	t_token	*curr;

	curr = token;
	if (ft_list_size(token) == 1 && curr->cmd && !ft_strcmp(curr->cmd, "export"))
	{
		sort_env_list(env);
		while (env)
		{
			printf("declare -x %s\n",env->line);
			env = env->next;
    	}
		return (1);
	}
    return (0);
}

void	respective_addition(t_env **env, t_env **copy, char *key, char *value, int flag)
{
	char	*val;
	if (flag == 1)
	{
		if (check_if_var_exist(copy, key))
			ft_update_env(key, value, env, copy);
		else
			ft_add_env(key, value, env, copy, 1);
		return ;
	}
	if (flag == 2)
	{
		if (check_if_var_exist(copy, key))
		{
			val = get_value_from_env(key, *copy);
			if (val)
				ft_update_env(key, val, env, copy);
		}
		else
			ft_add_key_to_env(copy, key);
		return ;
	}
}

void	ft_export(t_token *token, t_env **env)
{
	static 	t_env	*copy=NULL;
	t_token	*curr;
	char	*key;
	char	*value;
	int		i;

	curr = token;
	key = NULL;
	value = NULL;
	if (copy == NULL)
		copy = copy_env(*env);
	if (print_export_env(token, copy))
		return ;
	curr = curr->next;
	while (curr && curr->cmd)
	{
		ft_printf("I ENTERED\n");
		i = set_key_value(curr->cmd, &key, &value, copy);
		ft_printf("%s %s %d\n",key,value,i);
		if (i == 1)
		{
			if (has_equal(curr->cmd))
				respective_addition(env, &copy, key, value, 1);
			else
				respective_addition(env, &copy, key, value, 2);
		}
		if (i == 2)
			return ;
		curr = curr->next;
	}
}
