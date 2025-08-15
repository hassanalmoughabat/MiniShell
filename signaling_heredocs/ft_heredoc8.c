/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:39:13 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/08/13 12:05:04 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	init_heredoc_context(t_heredoc_ctx *ctx, t_token *curr,
		t_shell *shell)
{
	ctx->delimiter = get_delimeter(curr, &shell->gc);
	if (!ctx->delimiter)
		return (0);
	ctx->quote = has_quotes(ctx->delimiter);
	remove_added_quotes(&ctx->delimiter, &shell->gc);
	ctx->cmd = find_command_around_heredoc(shell->tk, ctx->delimiter);
	ctx->path = NULL;
	ctx->read_fd = -1;
	return (1);
}

void	cleanup_heredoc_context(t_heredoc_ctx *ctx)
{
	if (ctx->delimiter)
		free(ctx->delimiter);
	if (ctx->path)
		free(ctx->path);
	if (ctx->read_fd >= 0)
		close(ctx->read_fd);
}

void	execute_command(t_heredoc_ctx *ctx, t_shell *shell)
{
	char	*args[2];

	args[0] = ctx->cmd;
	args[1] = NULL;
	ctx->path = get_path(ctx->cmd, shell->ft_env, &shell->gc);
	if (ctx->read_fd < 0)
	{
		cleanup_heredoc_context(ctx);
		exit(1);
	}
	if (dup2(ctx->read_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		cleanup_heredoc_context(ctx);
		exit(1);
	}
	close(ctx->read_fd);
	if (ctx->path)
	{
		execve(ctx->path, args, shell->ft_env);
		free(ctx->path);
	}
	free(ctx->delimiter);
	exit(127);
}

void	child_process(t_heredoc_ctx *ctx, t_shell *shell)
{
	ctx->read_fd = handle_dless(ctx->delimiter, shell, ctx->quote);
	if (ctx->read_fd < 0)
	{
		free(ctx->delimiter);
		exit(130);
	}
	if (ctx->cmd)
		execute_command(ctx, shell);
	else
	{
		close(ctx->read_fd);
		free(ctx->delimiter);
		exit(0);
	}
}

void	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	ft_restore_main_signals();
	if (WIFSIGNALED(status))
		shell->env->exit_status = 128 + WTERMSIG(status);
	else
		shell->env->exit_status = WEXITSTATUS(status);
}
