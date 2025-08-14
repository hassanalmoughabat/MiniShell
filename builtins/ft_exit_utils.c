/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:04:11 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/25 14:25:28 by njoudieh42       ###   ########.fr       */
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
	if ((*shell)->shell_level > 2)
		(*shell)->shell_level --;
	else
		return ;
}

void	exit_args(bool in_range, t_shell *shell, char *cmd)
{
	if (ft_isnum(cmd) && in_range)
		shell->env->exit_status = ft_err_msg((t_error){"exit",
				ERROR_MESG_TOO_MANY_ARGS, ENU_GENEREAL_FAILURE});
	else
		shell->env->exit_status = ft_err_msg((t_error){cmd,
				ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
	return ;
}

void	exit_code(bool in_range, t_shell *shell, char *cmd)
{
	int	i;

	i = 0;
	if (ft_isnum(cmd) && in_range)
	{
		i = ft_atoi(cmd);
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
		shell->env->exit_status = ft_err_msg((t_error){cmd,
				ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
}

void	handle_exit_code(t_token *curr, t_shell *shell)
{
	long long	arg_num;
	bool		in_range;

	arg_num = 0;
	remove_added_quotes(&curr->cmd, NULL);
	if (!curr->cmd || ft_strlen(curr->cmd) == 0)
		shell->env->exit_status = ft_err_msg((t_error){"",
				ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
	in_range = ft_atoll_safe(curr->cmd, &arg_num);
	if (ft_list_size(curr) >= 2)
		exit_args(in_range, shell, curr->cmd);
	else if (ft_list_size(curr) == 1)
		exit_code(in_range, shell, curr->cmd);
	else
		shell->env->exit_status = ft_err_msg((t_error){curr->cmd,
				ERROR_MESG_NUMERIC_REQUIRED, ENU_MISUSED_BUILTIN});
	return ;
}
