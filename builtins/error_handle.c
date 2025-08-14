/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:24:55 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/24 16:49:18 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	ft_err_msg(t_error err)
{
	if (err.error_msg == ERROR_MESG_NOT_DIR)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.cmd, 2),
			ft_putstr_fd(": Not a directory \n", 2), err.error_num);
	else if (err.error_msg == ERROR_MESG_CMD_NOT_FOUND)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.cmd, 2),
			ft_putstr_fd(": command not found\n", 2), err.error_num);
	else if (err.error_msg == ERROR_MESG_NO_FILE)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.cmd, 2),
			ft_putstr_fd(": No such file or directory\n", 2), err.error_num);
	else if (err.error_msg == ERROR_MESG_PERMISSION_DENIED)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.cmd, 2),
			ft_putstr_fd(": Permission denied\n", 2), err.error_num);
	else if (err.error_msg == ERROR_MESG_TOO_MANY_ARGS)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.cmd, 2),
			ft_putstr_fd(": too many arguments\n", 2), err.error_num);
	else if (err.error_msg == ERROR_MESG_NUMERIC_REQUIRED)
		return (ft_putstr_fd("minishell: exit: ", 2),
			ft_putstr_fd(err.cmd, 2),
			ft_putstr_fd(": numeric argument required\n", 2), err.error_num);
	return (0);
}

int	ft_error_message_quotes(char c)
{
	ft_putstr_fd("Error unclosed quotes, close your ", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\n", 2);
	g_signal.sig_status = ENU_GENEREAL_FAILURE;
	return (-1);
}

void	error_message_export(char **str)
{
	if (*str)
		free(*str);
	return ;
}

void	error_validity_export(char *input, t_token *tk)
{
	if (input[0] == '-')
	{
		ft_putstr_fd("minishell export: ", 2);
		ft_putchar_fd('-', 2);
		ft_putchar_fd(input[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		return ;
	}
	remove_added_quotes(&tk->cmd, NULL);
	ft_putstr_fd("minishell export: ", 2);
	ft_putchar_fd('`', 2);
	ft_putstr_fd(tk->cmd, 2);
	ft_putchar_fd('\'', 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}
