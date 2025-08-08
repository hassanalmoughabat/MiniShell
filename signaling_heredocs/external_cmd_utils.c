/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:44:35 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 00:45:32 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = tokens;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

char	**build_args_array(t_token *cmd_tokens)
{
	char	**args;
	t_token	*curr;
	int		count;
	int		i;

	count = count_tokens(cmd_tokens);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	curr = cmd_tokens;
	while (curr)
	{
		remove_added_quotes(&curr->cmd);
		args[i] = ft_strdup(curr->cmd);
		if (!args[i])
		{
			ft_free_tab(args);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

void	print_cmd_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

int	symbols(char *tk)
{
	int		i;

	i = 0;
	while (tk[i])
	{
		if ((tk[i] == '/'))
			return (1);
		i ++;
	}
	return (0);
}
