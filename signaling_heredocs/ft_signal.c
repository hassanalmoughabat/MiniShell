/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 09:53:05 by njoudieh          #+#    #+#             */
/*   Updated: 2025/08/09 02:14:14 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	ft_heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_signal.heredoc_sigint = true;
	g_signal.sig_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_done = 1;
}

void	ft_sigint_handler(int num)
{
	(void)num;
	if (g_signal.signint_child)
		return ;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal.sig_status = 130;
}

void	ft_set_heredoc_signals(void)
{
	signal(SIGINT, ft_heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_sigquit_handler(int num)
{
	(void)num;
	if (g_signal.signint_child)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		g_signal.sig_status = 131;
	}
}

void	ft_init_signals(void)
{
	g_signal.heredoc_sigint = false;
	g_signal.signint_child = false;
	g_signal.sig_status = 0;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_set_child_signals(void)
{
	g_signal.signint_child = true;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ft_restore_parent_signals(void)
{
	g_signal.signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void ft_set_pipe_parent_signals(void)
{
    g_signal.signint_child = true;
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}
