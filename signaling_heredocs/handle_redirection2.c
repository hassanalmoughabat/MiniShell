/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:52:52 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 00:57:18 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

t_token	*create_new_token(t_token *src)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->cmd = ft_strdup(src->cmd);
	new->type = src->type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_token	*skip_redir_tokens(t_token *tk)
{
	if (tk->next)
		return (tk->next->next);
	return (tk->next);
}

t_token	*filter_cmd_tokens(t_token *tk)
{
	t_token	*res;
	t_token	*last;
	t_token	*new;

	res = NULL;
	last = NULL;
	while (tk)
	{
		if (tk->type == T_GREAT || tk->type == T_DGREAT
			|| tk->type == T_LESS || tk->type == T_DLESS)
		{
			tk = skip_redir_tokens(tk);
			continue ;
		}
		new = create_new_token(tk);
		if (!res)
			res = new;
		else
			last->next = new;
		last = new;
		tk = tk->next;
	}
	return (res);
}

int	open_output_file(t_token *curr, t_shell *shell)
{
	int	flags;
	int	tmp_fd;

	if (!is_valid_filename(curr->next))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putendl_fd(curr->next->cmd, 2);
		return (shell->env->exit_status = 2, 0);
	}
	remove_added_quotes(&curr->next->cmd);
	if (curr->type == T_GREAT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	tmp_fd = open(curr->next->cmd, flags, 0644);
	if (tmp_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(curr->next->cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (shell->env->exit_status = 1, 0);
	}
	close(tmp_fd);
	return (1);
}

int	open_file_input(t_token *curr, t_shell *shell)
{
	int	fd;

	if (!is_valid_filename(curr->next))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putendl_fd(curr->next->cmd, 2);
		shell->env->exit_status = 2;
		return (0);
	}
	remove_added_quotes(&curr->next->cmd);
	fd = open(curr->next->cmd, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(curr->next->cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		shell->env->exit_status = 1;
		return (0);
	}
	close(fd);
	return (1);
}
