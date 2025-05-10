/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 03:33:31 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/05 13:06:28 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	ft_check_quotes(char c)
{
	if (c == '\"' || c == '\'')
		return (1);
	return (0);
}

int	ft_delimeter(char *str)
{
	if (*str == ' ' || *str == '\t' || *str == '\n'
		|| !ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1)
		|| !ft_strncmp(str, "|", 1))
		return (1);
	return (0);
}
