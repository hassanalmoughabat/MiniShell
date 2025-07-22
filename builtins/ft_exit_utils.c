/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:04:11 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 16:15:09 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

bool	ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[0] == '-' || str[0] == '+')
		{
			i = 1;
			while (ft_isdigit(str[i]))
				i++;
			if (str[i] == '\0')
				return (true);
			return (false);
		}
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	decrement(t_shell **shell)
{
	if ((*shell)->shell_level > 1)
		(*shell)->shell_level --;
	else
		return ;
}

bool	ft_atoll_safe(const char *str, long long *result)
{
	int					sign = 1;
	unsigned long long	num = 0;
	unsigned long long	limit = LLONG_MAX;

	*result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			sign = -1;
			limit = -(unsigned long long)LLONG_MIN;
		}
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		int digit = *str - '0';
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

void	handle_exit_code(t_token *curr, t_shell *shell)
{
	int			i;
	long long	arg_num;
	bool		in_range;

	i = 0;
	arg_num = 0;
	remove_added_quotes(&curr->cmd);
	if (!curr->cmd || ft_strlen(curr->cmd) == 0)
		shell->env->exit_status = ft_err_msg((t_error){"",
				ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
	in_range = ft_atoll_safe(curr->cmd, &arg_num);
	if (ft_list_size(curr) >= 2)
	{
		if (ft_isnum(curr->cmd) && in_range)
			shell->env->exit_status = ft_err_msg((t_error){"exit",
						ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
		else
			shell->env->exit_status = ft_err_msg((t_error){curr->cmd,
						ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
	}
	else if (ft_list_size(curr) == 1)
	{
		if (ft_isnum(curr->cmd) && in_range)
		{
			i = ft_atoi(curr->cmd);
			if (i >= 0 && i <= 255)
				shell->env->exit_status = i;
			else if (i > 255)
				shell->env->exit_status = i % 256;
			else if (i < 0)
			{
				while (i < 0)
					i += 256;
				shell->env->exit_status = i;
			}
		}
		else
			shell->env->exit_status = ft_err_msg((t_error){curr->cmd,
						ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
	}
	else
		shell->env->exit_status = ft_err_msg((t_error){curr->cmd,
				ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
}
