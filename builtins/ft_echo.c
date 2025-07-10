/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:24:32 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/04 17:59:59 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	is_valid_n_flag(char *word)
{
	int	i;

	i = 1;
	if (!word || word[0] != '-' || word[1] == '\0')
		return (0);
	while (word[i] && word[i] == 'n')
		i++;
	return (word[i] == '\0');
}

char	*normalize_spaces(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		in_space;

	i = 0;
	j = 0;
	in_space = 0;
	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i] && ft_check_space(str[i]))
		i++;
	while (str[i])
	{
		if (ft_check_space(str[i]))
		{
			if (!in_space)
			{
				result[j++] = ' ';
				in_space = 1;
			}
		}
		else
		{
			result[j++] = str[i];
			in_space = 0;
		}
		i++;
	}
	if (j > 0 && result[j - 1] == ' ')
		j--;
	result[j] = '\0';
	return (result);
}

char	*extract_content_after_flags(char *str)
{
	char	*temp, *normalized, *result;
	int		i = 0;

	temp = ft_strdup(str);
	if (!temp)
		return (NULL);
	ft_skip_added_spaces(&temp);
	if (!is_quote_token(temp))
		remove_added_quotes(&temp);
	normalized = normalize_spaces(temp);
	free(temp);
	if (!normalized)
		return (NULL);
	while (normalized[i])
	{
		if (normalized[i] == '-')
		{
			int start = i++;
			while (normalized[i] && normalized[i] == 'n')
				i++;
			if (normalized[i] != ' ' && normalized[i] != '\0')
			{
				result = ft_strdup(normalized + start);
				free(normalized);
				return (result);
			}
			if (normalized[i] == ' ')
				i++;
		}
		else
		{
			result = ft_strdup(normalized + i);
			free(normalized);
			return (result);
		}
	}
	free(normalized);
	return (ft_strdup(""));
}

int	check_n_flags(char *str)
{
	char	*temp;
	int		i = 0, has_flags = 0, has_content = 0;

	if (!str)
		return (0);
	temp = ft_strdup(str);
	ft_skip_added_spaces(&temp);
	if (!is_quote_token(temp))
		remove_added_quotes(&temp);
	while (temp[i])
	{
		if (temp[i] == '-')
		{
			int start = i++;
			while (temp[i] && temp[i] == 'n')
				i++;
			if ((temp[i] == ' ' || temp[i] == '\0') && i > start + 1)
			{
				has_flags = 1;
				while (temp[i] == ' ')
					i++;
			}
			else
			{
				has_content = 1;
				break;
			}
		}
		else
		{
			has_content = 1;
			break;
		}
	}
	free(temp);
	return (has_flags ? (has_content ? 2 : 1) : 0);
}

int	is_quote_token(char *str)
{
	if (!str)
		return (0);
	return ((ft_strlen(str) == 1 && (str[0] == '"' || str[0] == '\'')) ||
			(ft_strlen(str) == 3 && str[0] == '"' && str[1] == '\'' && str[2] == '"') ||
			(ft_strlen(str) == 3 && str[0] == '\'' && str[1] == '"' && str[2] == '\''));
}

void	ft_echo(t_token *tk)
{
	t_token	*curr = tk;
	int		flag = 0;
	char	*temp, *content;

	if (curr && !ft_strcmp(curr->cmd, "echo"))
		curr = curr->next;
	while (curr)
	{
		int n_check = check_n_flags(curr->cmd);
		if (n_check == 1)
		{
			flag = 1;
			curr = curr->next;
		}
		else if (n_check == 2)
		{
			flag = 1;
			content = extract_content_after_flags(curr->cmd);
			if (content && content[0])
			{
				free(curr->cmd);
				curr->cmd = content;
				break ;
			}
			free(content);
			curr = curr->next;
		}
		else
			break ;
	}
	while (curr)
	{
		if (!curr->cmd)
			return ;
		temp = ft_strdup(curr->cmd);
		if (temp && ft_strcmp(temp, " "))
		{
			remove_added_quotes(&temp);
			if (temp && ft_strcmp(temp, ""))
			{
				ft_putstr_fd(temp, 1);
				ft_putstr_fd(" ", 1);
			}
		}
		free(temp);
		curr = curr->next;
	}
	if (!flag)
		ft_putstr_fd("\n", 1);
	g_minishell.env->exit_status = 0;
}
