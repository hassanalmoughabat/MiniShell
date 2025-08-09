/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 09:53:05 by njoudieh          #+#    #+#             */
/*   Updated: 2025/08/09 15:22:38 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_signal.sig_status = 128 + SIGINT;
	exit(128 + SIGINT);
}

void	ft_sigint_handler(int num)
{
	(void)num;
	if (g_signal.signint_child)
	{
		ft_putstr_fd("\n", 1);
		g_signal.signint_child = false;
		g_signal.heredoc_sigint = true;
	}
	else
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	g_signal.sig_status = 130;
}

void	ft_sigquit_handler(int num)
{
	(void)num;
	ft_putstr_fd("Quit: 3\n", 1);
}

void	ft_init_signals(void)
{
	g_signal.heredoc_sigint = false;
	g_signal.signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
