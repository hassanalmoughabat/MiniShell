/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by claude            #+#    #+#             */
/*   Updated: 2025/08/06 20:50:23 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "../includes/minihell.h"

typedef struct s_pipe_redirect_params
{
	int			i;
	int			**pipes;
	int			pipe_count;
	int			heredoc_fd;
	int			is_first_with_heredoc;
}				t_pipe_redirect_params;

typedef struct s_heredoc_pipe_params
{
	pid_t		pid1;
	int			*pipefd;
	char		*delimiter;
	t_token		*redirect_token;
	char		**ft_env;
	t_env		*env;
}				t_heredoc_pipe_params;

typedef struct s_heredoc_child_params
{
	int			*pipefd;
	char		*delimiter;
	t_env		*env;
	char		**ft_env;
	int			quote;
}				t_heredoc_child_params;

typedef struct s_child_setup_params
{
	t_token		*cmd_segment;
	int			i;
	int			heredoc_fd;
}				t_child_setup_params;

void			pipe_error_message(char *input, t_token *tk);
t_token			*remove_redirection_tokens(t_token *cmd_segment);
void			setup_input_redirection(t_pipe_child_data *child_data);
void			setup_output_redirection(t_pipe_child_data *child_data);
void			handle_redirections_and_execute(t_pipe_child_data *child_data,
					t_shell *shell);
void			remove_heredoc_tokens(t_pipe_child_data *child_data);
void			add_token_to_list(t_token **new_list, t_token **last,
					t_token *new_token);
t_token			*create_token_copy(t_token *src);
int				has_redirect_after_pipe(t_token *lst);
int				has_heredoc_before_pipe(t_token *lst);
void			close_parent_heredoc(t_pipe_data *data, int heredoc_fd);
void			setup_child_data(t_pipe_child_data *child_data,
					t_pipe_data *data, t_child_setup_params *params);
void			close_unused_heredocs(t_pipe_data *data, int heredoc_fd);
int				setup_heredoc_for_command(t_pipe_data *data, t_token *start,
					t_token *curr);
void			cleanup_heredocs(t_heredoc_info *heredocs, int count);
int				process_single_heredoc(t_token *curr, t_heredoc_info *heredocs,
					t_shell *shell, int i);
void			find_tokens(t_token *lst, t_token **heredoc_token,
					t_token **pipe_token, t_token **redirect_token);
pid_t			create_heredoc_child(t_heredoc_child_params *params,
					t_shell *shell);
int				setup_heredoc_pipe(t_token *lst, char **ft_env, t_env *env,
					t_heredoc_pipe_params *pipe_params);
int				handle_heredoc_pipe_redirect(t_token *lst, t_shell *shell);
void			setup_output_redirect_child(int *pipefd,
					t_token *redirect_token, char **ft_env);
pid_t			create_output_child(int *pipefd, t_token *redirect_token,
					char **ft_env);
void			cleanup_heredocs_after_exec(t_pipe_data *data);
void			wait_for_children(t_pipe_data *data, t_shell *shell);
void			execute_single_command(t_pipe_data *data, t_token *start,
					t_token *curr, int i, t_shell *shell);
void			setup_pipe_data(t_pipe_data *data, t_token *lst,
					t_shell *shell);
void			handle_great_redirect(t_token *curr);
void			handle_dgreat_redirect(t_token *curr);
void			setup_input_redirection(t_pipe_child_data *child_data);
t_token			*remove_redirection_tokens(t_token *cmd_segment);
// From pipes_utils.c
int				has_heredoc(t_token *lst);
void			close_all_pipes(int **fds, int count);
t_token			*next_pipe(t_token *start);
int				count_pipes(t_token *lst);
int				create_pipes(int ***pipes, int pipe_count);

// From pipes_heredoc.c
t_heredoc_info	*process_heredocs_before_pipes(t_token *lst,
					t_shell *shell, int *hd_count);
// norminette for

int				segment_has_heredoc(t_token *start, t_token *end);
int				get_heredoc_fd_for_segment(t_token *start, t_token *end,
					t_heredoc_info *heredocs, int hd_count);
int				handle_heredoc_pipe_redirect_part2(
					t_heredoc_pipe_params *params, t_shell *shell);

// From pipes_segment.c
int				ft_pipe_builtin(t_token *tk);
t_token			*extract_command_segment(t_token *start, t_token *end);
void			setup_pipe_redirects(t_pipe_redirect_params *params);
void			handle_pipe_child(t_pipe_child_data *child_data,
					t_shell *shell);
// int				valid_pipe(t_token *tk);

// From pipes_redirect.c
int				check_special_heredoc_pipe(t_token *lst, t_shell *shell);

// From pipes_main.c
void			execute_pipe_commands(t_pipe_data *data, t_shell *shell);
void			handle_pipe(t_token *lst, t_shell *shell, char *input);

#endif