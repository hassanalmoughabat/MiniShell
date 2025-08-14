/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:35:30 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/24 16:46:40 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i ++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_list_size(t_token *token)
{
	t_token	*current;
	int		size;

	size = 0;
	current = token;
	while (current)
	{
		size ++;
		current = current->next;
	}
	return (size);
}

void	print_helper(t_env *env, t_gc *gc)
{
	char	*pwd;
	char	*pwd_env;
	char	*temp_env;

	temp_env = get_value_from_env("_=", env, NULL);
	if (temp_env)
	{
		ft_putstr_fd(temp_env, 1);
		free(temp_env);
	}
	ft_putstr_fd("\n", 1);
	pwd = ft_get_pwd(gc);
	if (pwd)
	{
		pwd_env = ft_strjoin("PWD=", pwd, gc);
		if (pwd_env)
		{
			ft_putstr_fd(pwd_env, 1);
			ft_putstr_fd("\n", 1);
		}
	}
	ft_putstr_fd("SHLVL=1\n", 1);
	ft_putstr_fd("_=\n", 1);
}

void	ft_print_env(t_env *env, t_gc *gc)
{
	t_env	*ev;

	ev = env;
	if (!ev)
		print_helper(env, gc);
	else
	{
		while (ev)
		{
			ft_putstr_fd(ev->line, 1);
			ft_putstr_fd("\n", 1);
			ev = ev->next;
		}
	}
	env->exit_status = ENU_SUCCESS;
}

void	display_list(t_token *tk)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		ft_putstr("line is ");
		ft_putstr(curr->cmd);
		ft_putstr("\n");
		curr = curr->next;
	}
}
