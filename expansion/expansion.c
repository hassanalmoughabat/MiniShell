/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:15:55 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/08 23:11:58 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int should_expand_dollar(char *str) {
    int i;
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    
    if (!str)
        return (0);
    i = 0;
    while (str[i])
	{
        if (str[i] == '\\' && str[i + 1]) {
            i += 2;
            continue;
        }
        if (str[i] == '\'' && !in_double_quotes)
            in_single_quotes = !in_single_quotes;
        else if (str[i] == '"' && !in_single_quotes)
            in_double_quotes = !in_double_quotes;
        else if (str[i] == '$')
		{
            if (!in_single_quotes)
                return (1);
        }
        i++;
    }
    return (0);
}
int is_dollar_inside_single_quotes(char *str, int dollar_pos)
{
	int i = 0;
	int in_single = 0;
	int in_double = 0;

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

void	replace_dollar(t_token **t_list)
{
	t_token		*curr;
	char		*temp;

	temp = NULL;
	if (!t_list || !*t_list)
		return ;
	curr = *t_list;
	while (curr)
	{
		if (curr->prev && ft_strcmp(curr->prev->cmd, "<<") == 0)
		{
			curr = curr->next;
			continue;
		}
		if (ft_has_dollar(curr->cmd))
		{
			if (!should_expand_dollar(curr->cmd))
				temp = skip_dollars_before_quotes(curr->cmd);
			else
				handle_value(curr->cmd, &temp);
			if (temp)
			{
				free(curr->cmd);
				curr->cmd = ft_strdup(temp);
				free(temp);
			}
			else
			{
				free(curr->cmd);
				curr->cmd = ft_strdup("");
			}
		}
		curr = curr->next;
	}
}

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

int	expand_helper(char **substr, int flag)
{
	int		dollar_index;
	char	*temp;

	dollar_index = find_dollar_index(*substr);
	if (quote_type(*substr) == 1)
	{
		if (dollar_index != -1)
		{
			if (is_dollar_inside_single_quotes(*substr, dollar_index))
			{
			}
			else
			{
				temp = handle_dollar(*substr, flag);
				free(*substr);
				*substr = temp;
			}
		}
	}
	else if (quote_type(*substr) == 0 || quote_type(*substr) == 2 || quote_type(*substr) == 3)
	{
		if (dollar_index != -1)
		{
			if (dollar_index > 0 && (*substr)[dollar_index - 1] == '\"'
			&& (*substr)[dollar_index + 1] == '\"')
			{
				free(*substr);
				*substr = ft_strdup("\"$\"");
			}
			else
			{
				temp = handle_dollar(*substr, flag);
				free(*substr);
				*substr = temp;
			}
		}
	}
	return (1);
}
