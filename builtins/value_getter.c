/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value_getter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:16:46 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/22 22:50:58 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	has_equal_in_quote(char *input, char *quote, t_gc *gc)
{
	int		i;
	int		j;
	char	*substr;

	i = 0;
	while (input[i])
	{
		if (ft_check_quotes(input[i]) && !escape(input, i))
		{
			*quote = input[i];
			j = ++ i;
			while (input[i] && (input[i] != *quote || escape(input, i)))
				i++;
			substr = ft_substr(input, j, i - j, gc);
			if (has_equal(substr))
				return (1);
		}
		i ++;
	}
	return (0);
}

char	*get_value(char *input, char quote, int flag, t_shell *shell)
{
	char			*value;
	char			*result;
	t_value_params	vparams;

	value = ft_strdup_gc(&shell->gc, "");
	if (!input)
		return (NULL);
	vparams.value = &value;
	vparams.quote = quote;
	vparams.input = input;
	vparams.flag = flag;
	vparams.gc = &shell->gc;
	set_value(&vparams);
	if (!value)
		return (NULL);
	handle_value(value, &result, shell);
	if (remove_added_quotes(&result, &shell->gc) == -1)
		return (NULL);
	return (result);
}

char	*get_key(t_token *tk, t_shell *shell, char *quote, int *ind)
{
	char			*result;
	char			*key;
	t_key_params	kparams;

	result = ft_strdup_gc(&shell->gc, "");
	if (!shell->curr_cmd)
		return (NULL);
	kparams.input = shell->curr_cmd;
	kparams.result = &key;
	kparams.quote = quote;
	kparams.flag = ind;
	kparams.gc = &shell->gc;
	set_key(&kparams);
	if (!check_valid_key(tk, key, shell->env, &shell->gc))
		return (NULL);
	handle_value(key, &result, shell);
	if (remove_added_quotes(&result, &shell->gc) == -1)
		return (NULL);
	if (!check_key_after_expansion(result, shell->env, tk))
		return (NULL);
	return (result);
}
