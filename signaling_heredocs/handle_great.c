/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_great.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/04/11 10:00:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	open_output_file(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	return (fd);
}

 int	handle_standalone_redirect(char *filename, t_token *tk, int flags)
{
	int	fd;

	if (tk && tk->cmd && (ft_strcmp(tk->cmd, ">") == 0
			|| ft_strcmp(tk->cmd, ">>") == 0))
	{
		fd = open_output_file(filename, flags);
		if (fd == -1)
			return (0);
		close(fd);
		return (1);
	}
	return (-1);
}

 t_token	*find_redirect_token(t_token *tk, char *redirect_op)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if (curr->cmd && ft_strcmp(curr->cmd, redirect_op) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

t_token *copy_tokens_before_redirect(t_token *tk, t_token *redirect_token)
{
    t_token *cmd_tokens = NULL;
    t_token *current = tk;
    t_token *last_cmd = NULL;
    t_token *new_token;

    while (current && current != redirect_token)
    {
        new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            free_token_list(cmd_tokens);  // ❌ This might be causing issues
            return NULL;
        }
        new_token->cmd = ft_strdup(current->cmd);  // ✅ Good - copies string
        new_token->type = current->type;
        new_token->next = NULL;
        new_token->prev = NULL;  // ❌ Add this
        
        if (!cmd_tokens)
            cmd_tokens = new_token;
        else
            last_cmd->next = new_token;
        last_cmd = new_token;
        current = current->next;
    }
    return cmd_tokens;
}

void	execute_builtin_redirect(t_token *cmd_tokens, char **ft_env,
		t_env *env)
{
	handle_builtin(cmd_tokens, ft_env, &env);
	free_token_list(cmd_tokens);
	exit(EXIT_SUCCESS);
}
