/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:18 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/25 14:56:54 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

static const char	*ft_skip_spaces(const char *str)
{
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	return (str);
}

static const char	*ft_parse_sign(const char *str,
	int *sign, unsigned long long *limit)
{
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			*sign = -1;
			*limit = -(unsigned long long)LLONG_MIN;
		}
		str++;
	}
	return (str);
}

bool	ft_atoll_safe(const char *str, long long *result)
{
	unsigned long long	num;
	unsigned long long	limit;
	int					sign;
	int					digit;

	limit = LLONG_MAX;
	num = 0;
	sign = 1;
	*result = 0;
	str = ft_parse_sign(ft_skip_spaces(str), &sign, &limit);
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (num > (limit - digit) / 10)
			return (false);
		num = num * 10 + digit;
		str++;
	}
	if (*str != '\0')
		return (false);
	*result = (long long)(sign * num);
	return (true);
}

void	ft_exit(t_shell *shell)
{
	t_token	*curr;
	int		exit_code;

	printf("exit\n");
	curr = shell->tk;
	if (!curr)
		exit_code = shell->env->exit_status;
	else
	{
		decrement(&shell);
		update_shlvl_in_env(&(shell->env), shell->shell_level);
		if (curr && curr->cmd)
		{
			curr = curr->next;
			if (!curr)
				shell->env->exit_status = 0;
			else
				handle_exit_code(curr, shell);
		}
		exit_code = shell->env->exit_status;
	}
	free_shell(shell);
	free_array(shell->ft_env);
	exit(exit_code);
}
