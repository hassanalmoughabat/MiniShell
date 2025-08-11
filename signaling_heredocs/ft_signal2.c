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

void	ft_restore_parent_signals(void)
{
	g_signal.signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_set_child_signals(void)
{
	g_signal.signint_child = true;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ft_set_pipe_parent_signals(void)
{
	g_signal.signint_child = true;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
