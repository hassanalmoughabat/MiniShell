/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_extraction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 21:15:45 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/21 21:01:35 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*extract_and_expand(char *input, t_shell *shell)
{
	int		i;
	int		flag;
	char	*expanded;
	char	*substr;

	i = 0;
	flag = 0;
	expanded = ft_strdup("");
	while (input[i])
	{
		if (ft_check_quotes(input[i]) && !escape(input, i))
			substr = extract_quoted_substring(input, &i);
		else
			substr = extract_unquoted_substring(input, &i, &flag);
		if (!substr)
			continue ;
		expand_helper(&substr, flag, shell);
		expanded = ft_strjoin(expanded, substr);
		free(substr);
	}
	return (expanded);
}

void	handle_value(char *value, char **result, t_shell *shell)
{
	char	*temp;

	temp = NULL;
	temp = extract_and_expand(value, shell);
	if (temp)
	{
		*result = ft_strdup(temp);
		free(temp);
	}
}
