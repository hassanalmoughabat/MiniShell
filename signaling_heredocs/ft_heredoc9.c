/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc9.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:50:37 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/12 20:10:29 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_check_quotes(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	process_heredoc_token(t_token *curr, t_shell *shell)
{
	t_heredoc_ctx	ctx;
	pid_t			pid;

	if (!init_heredoc_context(&ctx, curr, shell))
		return (0);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(ctx.delimiter);
		shell->env->exit_status = 1;
		return (0);
	}
	if (pid == 0)
		child_process(&ctx, shell);
	else
	{
		free(ctx.delimiter);
		handle_parent_process(pid, shell);
		if (shell->env->exit_status == 130)
			return (0);
	}
	return (1);
}

void	handle_heredoc(t_shell *shell)
{
	t_token	*curr;

	curr = shell->tk;
	while (curr)
	{
		if (curr && curr->cmd && ft_strcmp(curr->cmd, "<<") == 0
			&& curr->next)
		{
			if (!process_heredoc_token(curr, shell))
				return ;
			curr = curr->next->next;
		}
		else
			curr = curr->next;
	}
}
