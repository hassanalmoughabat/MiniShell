/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:15:55 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/26 16:38:12 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

void	update_quotes_state(char c, int *in_s, int *in_d)
{
	if (c == '\'' && !*in_d)
		*in_s = !*in_s;
	else if (c == '"' && !*in_s)
		*in_d = !*in_d;
}

int	should_expand_dollar(char *str)
{
	int	i;
	int	in_s;
	int	in_d;

	if (!str)
		return (0);
	i = 0;
	in_s = 0;
	in_d = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i += 2;
			continue ;
		}
		update_quotes_state(str[i], &in_s, &in_d);
		if (str[i] == '$' && !in_s)
			return (1);
		i++;
	}
	return (0);
}

int	is_dollar_inside_single_quotes(char *str, int dollar_pos)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_double = 0;
	in_single = 0;
	if (!str || str[dollar_pos] != '$')
		return (0);
	while (i < dollar_pos)
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i += 2;
			continue ;
		}
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single);
}

char	*skip_dollars_before_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
		result[j++] = str[i++];
	result[j] = '\0';
	return (result);
}

char	*expand_token_value(char *cmd, t_shell *shell)
{
	char	*temp;

	temp = NULL;
	if (!should_expand_dollar(cmd))
		temp = skip_dollars_before_quotes(cmd);
	else
		handle_value(cmd, &temp, shell);
	return (temp);
}
