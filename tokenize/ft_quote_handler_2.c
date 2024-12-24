/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote_handler_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:24:37 by njoudieh42        #+#    #+#             */
/*   Updated: 2024/12/24 21:57:51 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/token.h"

char	*ft_trim_quotes(char *str, int i, int j)
{
	char	*temp;
	int		k;
	
	k = 0;
	temp = ft_calloc(j - i + 2, sizeof(char));
    if (!temp)
        return (0);
    while (i <= j)
    {
        temp[k] = str[i];
        i++;
        k++;
    }
    return (temp);
}

char	*ft_handle_single_quotes(char **input, int **i, int **j)
{
		char	*temp;
	char	*str;
	int		temp2;
	
	temp2 = *i + 1;
	while (*input[temp2] != '\'' && *input[temp2])
		temp2++;
	if (!*input[temp2])
		return (-1);
	if (!ft_check_space(*input[temp2 + 1]))
	{
		temp2++;
		while (!ft_check_space(*input[temp2]))
			temp2++;
	}
	temp = ft_trim_quotes(*input, **i, temp2);
	temp2++;
	**j = temp2;
	return (temp);
}

char	*ft_handle_double_quotes(char **input, int **i, int **j)
{
	char	*temp;
	char	*str;
	int		temp2;
	
	temp2 = *i + 1;
	while (*input[temp2] != '\"' && *input[temp2])
		temp2++;
	if (!*input[temp2])
		return (-1);
	if (!ft_check_space(*input[temp2 + 1]))
	{
		temp2++;
		while (!ft_check_space(*input[temp2]))
			temp2++;
	}
	temp = ft_trim_quotes(*input, **i, temp2);
	temp2++;
	**j = temp2;
	return (temp);
}

char	*ft_handle_word(char **input, int **i, int **j)
{
	char	*temp;

	temp = NULL;
	while (!ft_check_space(input[**j]))
		(**j)++;
	(**j)--;
	temp = ft_trim_quotes(*input, **i, **j);
	while (ft_check_space(input[**j]))
		(**j)++;
	i = j;
	j++;
	return (temp);
}

int	ft_check_type_quote(char c)
{
	if (c == '\'')
		return (T_Squote);
	else if (c == '\"')
		return (T_Dquote);
	return (0);
}
