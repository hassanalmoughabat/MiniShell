/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_setter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 00:31:49 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/24 21:06:57 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

char	*handle_flag(char *equal_pos, char quote, t_gc *gc)
{
	char	*first_quote;
	char	*new_value;
	size_t	len;

	len = 0;
	first_quote = ft_strchr(equal_pos, quote);
	len = first_quote - equal_pos;
	new_value = ft_malloc(gc, len + 3);
	if (!new_value)
		return (NULL);
	new_value[0] = quote;
	ft_strlcpy(new_value + 1, equal_pos, len + 1);
	new_value[len + 3] = '\0';
	if ((*first_quote + 1))
		new_value = ft_strjoin(new_value, first_quote, gc);
	return (new_value);
}

void	set_value(char **value, char quote, char *input, int flag, t_gc *gc)
{
	char	*equal_pos;
	char	*new_value;

	equal_pos = ft_strchr(input, '=');
	equal_pos ++;
	if (ft_check_space(*equal_pos))
		*value = ft_strdup_gc(gc, "\"\"");
	else
	{
		if (flag)
			new_value = handle_flag(equal_pos, quote, gc);
		else
			new_value = ft_strdup_gc(gc, equal_pos);
		*value = new_value;
	}
	return ;
}

void	set_key(char *input, char **result, char *quote, int *flag, t_gc *gc)
{
	char	*equal_ptr;
	char	*temp;

	equal_ptr = ft_strchr(input, '=');
	*result = ft_substr(input, 0, equal_ptr - input, gc);
	*flag = has_equal_in_quote(input, quote, gc);
	if (*flag == 1)
	{
		temp = ft_strjoin_char(*result, *quote, gc);
		*result = temp;
	}
	return ;
}

int	set_key_value(t_token *tk, char **key, char **value, t_shell *shell)
{
	shell->curr_cmd = tk->cmd;
	if (!shell->curr_cmd || shell->curr_cmd[0] == '\0'
		|| !ft_strcmp(tk->cmd, ""))
		return (*key = NULL, 1);
	if (has_equal(shell->curr_cmd) && ft_strlen(shell->curr_cmd) > 1)
	{
		if (equal_handler_export(tk, key, value, shell) == -1)
			return (-1);
		return (1);
	}
	*key = ft_strdup(shell->curr_cmd);
	if (*key && !check_valid_key(tk, *key, shell->env, &shell->gc))
		return (free(*key), -1);
	if (remove_added_quotes(key, NULL) == -1 || !ft_strcmp(*key, ""))
		return (error_message_export(key), -1);
	if (*key[0] == '#')
	{
		*key = NULL;
		*value = NULL;
		return (0);
	}
	return (1);
}
