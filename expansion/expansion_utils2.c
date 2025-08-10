/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:36:30 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 16:37:28 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	find_dollar_index(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '$' && !escape(str, i))
			return (i);
		i++;
	}
	return (-1);
}

void	replace_with_dollar(char **substr, int flag, t_shell *shell)
{
	char	*temp;

	temp = handle_dollar(*substr, flag, shell);
	free(*substr);
	*substr = temp;
}

void	handle_quoted_case(char **substr, int flag, t_shell *shell,
	int d_idx)
{
	if (is_dollar_inside_single_quotes(*substr, d_idx))
		return ;
	replace_with_dollar(substr, flag, shell);
}

void	handle_other_cases(char **substr, int flag, t_shell *shell,
	int d_idx)
{
	if (d_idx > 0 && (*substr)[d_idx - 1] == '"' && (*substr)[d_idx + 1] == '"')
	{
		free(*substr);
		*substr = ft_strdup("\"$\"");
	}
	else
		replace_with_dollar(substr, flag, shell);
}

int	expand_helper(char **substr, int flag, t_shell *shell)
{
	int	d_idx;
	int	q_type;

	d_idx = find_dollar_index(*substr);
	q_type = quote_type(*substr);
	if (d_idx == -1)
		return (1);
	if (q_type == 1)
		handle_quoted_case(substr, flag, shell, d_idx);
	else if (q_type == 0 || q_type == 2 || q_type == 3)
		handle_other_cases(substr, flag, shell, d_idx);
	return (1);
}
