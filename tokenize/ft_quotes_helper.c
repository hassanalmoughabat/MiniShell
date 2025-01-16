/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 03:33:31 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/01/09 14:56:16 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	ft_check_quotes(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	ft_delimeter(char *str)
{
	if (*str == ' ' || *str == '\t' || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "<", 1) || !ft_strncmp(str, "|", 1))
		return (1);
	return (0);
}
