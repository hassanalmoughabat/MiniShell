/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:21:07 by hal-moug          #+#    #+#             */
/*   Updated: 2025/08/09 02:39:24 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHELL_H
# define MINIHELL_H
# define MAX_HEREDOC_SIZE 16384
# include "ft_printf/ft_printf.h"
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../tokenize/token.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <linux/limits.h>
# include <signal.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>

typedef struct s_env
{
	char			*line;
	int				exit_status;
	struct s_env	*next;
}	t_env;

typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

typedef struct s_garbage_collector
{
	t_mem_node	*head;
}	t_gc;


typedef struct s_shell
{
	t_env		*env;
	char		*curr_cmd;
	char		*path;
	t_token		*tk;
	t_env		*copy;
	char		*key;
	char		*value;
	char		**ft_env;
	int			shell_level;
	t_gc		gc;
}	t_shell;



typedef struct s_signal
{
	bool		signint_child;
	bool		heredoc_sigint;
	int			sig_status;
}	t_signal;

extern t_signal	g_signal;
typedef enum error_msg
{
	ERROR,
	ERROR_MESG_CMD_NOT_FOUND,
	ERROR_MESG_NO_FILE,
	ERROR_MESG_PERMISSION_DENIED,
	ERROR_MESG_TOO_MANY_ARGS,
	ERROR_MESG_NUMERIC_REQUIRED,
	ERROR_MESG_NOT_DIR
}	t_err_msg;

typedef enum error_num
{
	ENU_SUCCESS,
	ENU_GENEREAL_FAILURE,
	ENU_MISUSED_BUILTIN,
	ENU_CMD_CANT_EXECUTE = 126,
	ENU_CMD_NOT_FOUND,
	ENU_OUT_OF_RANGE = 255
}	t_err_num;

typedef struct s_err
{
	char			*cmd;
	t_err_msg		error_msg;
	t_err_num		error_num;
}	t_error;

typedef struct s_pip
{
	t_token	*cmd;
	t_token	*context;
	int		*pipes;
	int		pipe_count;
	int		cmd_index;
}	t_pip;

typedef struct s_heredoc_data
{
	int		pipefd[2];
	size_t	total_written;
	char	*delimiter;
	int		quote;
}	t_heredoc_data;

typedef struct s_heredoc_info
{
	int		fd;
	t_token	*position;
}	t_heredoc_info;

typedef struct s_expand_data
{
	char	*result;
	char	*dst;
	int		result_len;
	int		buffer_size;
}	t_expand_data;

typedef struct s_heredoc_ctx
{
	char	*delimiter;
	char	*cmd;
	char	*path;
	int		quote;
	int		read_fd;
}	t_heredoc_ctx;

typedef struct s_pipe_data
{
	t_token			*lst;
	char			**ft_env;
	t_env			*env;
	int				**pipes;
	int				pipe_count;
	t_heredoc_info	*heredocs;
	int				hd_count;
	pid_t			last_child_pid;
}	t_pipe_data;

typedef struct s_pipe_child_data
{
	t_token	*cmd_segment;
	char	**ft_env;
	t_env	*env;
	int		i;
	int		**pipes;
	int		pipe_count;
	int		heredoc_fd;
	int		is_first_with_heredoc;
}	t_pipe_child_data;


void	init_gc(t_gc *gc);
void	*ft_malloc(t_gc *gc, size_t size);
void	ft_free_all(t_gc *gc);
char	*ft_strdup_gc(t_gc *gc, const char *s);
char	*ft_strjoin_gc(const char *s1, const char *s2, t_gc *gc);
// -------------------quote/token checks----------------------------
int		is_quote_token(char *str);
bool	contains_quote(const char *str);
int		contain_char(char *str, char c);
int		is_valid_filename(t_token *token);

// --------------value getter setter----------------------------
void	set_value(char **value, char quote, char *input, int flag, t_gc *gc);
void	set_key(char *input, char **result, char *quote, int *flag, t_gc *gc);
char	*get_key(t_token *tk, t_shell *shell, char *quote, int *ind);
char	*get_value(char *input, char quote, int flag, t_shell *shell);
int		set_key_value(t_token *tk, char **key, char **value, t_shell *shell);

// --------------------cd----------------------------------------
char	*ft_get_pwd(t_gc *gc);
char	*ft_get_cd_pwd(void);
void	ft_cd(t_shell *shell);
int		ft_find_old_pwd(char *str);
char	*ft_get_old_pwd(t_env *env);
char	*set_curr_pwd(t_shell *shell);
char	*find_dir_in_list(t_token *tk);
void	error_env(t_shell *shell, int flag);
int		change_dir(char *dir, t_shell *shell);
void	retrieve_dir(t_shell *shell, char *old_pwd);
void	update_env_values(t_shell *shell, char *old_pwd);

// --------------------echo---------------------------------------
void	ft_echo(t_shell *shell);
int		check_n_flags(char *str, t_gc *gc);
char	*normalize_spaces(char *str, t_gc *gc);
char	*extract_content_after_flags(char *str, t_gc *gc);
void	handle_n_cases(t_token **tk, int *flag, t_gc *gc);
// --------------------export-------------------------------------
int		has_equal(char *input);
void	ft_export(t_shell *shell, t_env **copy);
void	ft_add_key_to_env(t_env **copy, char *key, t_gc *gc);
int		check_if_var_exist(t_env **env, char *key);
char	*get_value_from_env(char *key, t_env *env, t_gc *gc);
int		equal_handler_export(t_token *tk, char **key,
			char **value, t_shell *shell);
int		ft_update_env(t_env **copy, t_shell *shell);
char	*quotes_in_env(char *value, char *key, int flag, t_gc *gc);
int		check_valid_key(t_token *tk, char *key, t_env *env, t_gc *gc);
int		print_export_env( t_shell *shell, int flag, t_env *copy);
int		handle_export_quotes(char *temp, char *result, size_t *i, size_t *j);

// ---------------------env------------------------------------------
char	*get_var(char *input, t_gc *gc);
size_t	extract_key_env(char *line);
char	*my_getenv(char *name, char **env);
void	ft_push_to_env(t_env **env, t_env *node);
void	ft_add_env(t_env **copy, t_shell *shell, int flag);

// ----------------------pwd-----------------------------------------
int		ft_pwd(t_shell *shell);

// ----------------------helpers for builtins------------------------
void	ft_print_env(t_env *env, t_gc *gc);
void	display_list(t_token *tk);
int		ft_list_size(t_token *token);
int		ft_strcmp(char *s1, char *s2);
void	print_env_list(t_env *env_list);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strcpy(char *dest, const char *src);
void	handle_builtin(t_shell *shell, char *cmd);

// ----------------------copy env-------------------------------------
t_env	*copy_env(t_env *envp, t_gc *gc);
char	**transform(t_env *env, t_gc *gc);
void	ft_free_env(t_env *head);
t_env	*new_env_node(char *key_value, t_gc *gc);

// ----------------------get path-------------------------------------
char	*get_my_path(t_env *env, t_gc *gc);
t_env	*create_env_node(char *envp, t_gc *gc);
t_env	*initialize_env_list(char **envp, t_gc *gc);
char	*get_path(char *cmd, char *envp[], t_gc *gc);
char	*path_extract(char *str, int count, t_gc *gc);

// ----------------------free functions-------------------------------
void	free_array(char **arr);
void	free_my_env(t_env *env);
void	ft_free_tab(char **tab);
void	free_env_list(t_env *head);
void	free_shell(t_shell *shell);
void	free_shell_args(char **args);
void	free_token_list(t_token *head);
void	cleanup_minishell(t_shell *shell, char **transform_result, char *input);

// ----------------------error messages--------------------------------
int		ft_err_msg(t_error err);
int		ft_error_message_quotes(char c);
void	error_message_export(char **str);
void	error_validity_export(char *input, t_token *tk);

// ----------------------expansion-------------------------------------
int		ft_has_dollar(char *str);
char	*ft_strjoin_char(char *str, char c, t_gc *gc);
char	*extract_value(char *str, int *index, t_gc *gc);
int		ft_check_dollar(char *value, int index);
int		ft_check_exceptions(char *str, int index);
char	*extract_dollar_var(char *key, int *index, t_gc *gc);
char	*join_char_and_free(char *expanded, char c, t_gc *gc);
char	*join_env_value(char *expanded, char *value);
char	*expand_token_value(char *cmd, t_shell *shell);
void	replace_dollar(t_token **t_list, t_shell *shell);
int		handle_standalone_dollar_check(char *key, int i);
void	update_quotes_state(char c, int *in_s, int *in_d);
char	*handle_dollar(char *key, int flag, t_shell *shell);
int		expand_helper(char **substr, int flag, t_shell *shell);
void	handle_value(char *value, char **result, t_shell *shell);
int		is_dollar_inside_single_quotes(char *str, int dollar_pos);
void	process_dollar_char(char *key, int *i, char **expanded,
			t_shell *shell);
char	*handle_var_expansion(char *expanded, char *key, int *i,
			t_shell *shell);
void	update_quotes(int *s_quotes, int *d_quotes, char *key, int i);
int		check_key_after_expansion(char *key, t_env *env, t_token *tk);
int		handle_dollar_in_quotes(char *key, int i, int in_s);
int		dollar_cases(char *key, int *index, char **expanded, t_shell *shell);
int		handle_standalone_dollar(char *key, int i);
// -----------------------exit------------------------------------------
void	ft_exit(t_shell *shell);
void	handle_exit_code(t_token *curr, t_shell *shell);
bool	ft_atoll_safe(const char *str, long long *result);

// -----------------------shell level-----------------------------------
void	decrement(t_shell **shell);
void	add_shell_level(t_env **env, char *new_line, t_gc *gc);
void	update_shlvl_in_env(t_env **env, int new_shlvl);

// ------------------------quotes----------------------------------------
int		quote_type(char *str);
int		remove_added_quotes(char **value, t_gc *gc);
int		has_equal_in_quote(char *input, char *quote, t_gc *gc);
char	*extract_quoted_substring(char *input, int *i, t_gc *gc);
char	*extract_unquoted_substring(char *input, int *i, int *flag, t_gc *gc);

// -----------------------unset------------------------------------------
void	ft_unset(t_shell *shell, t_env **copy);
void	update_env_value(t_env **head, char *target, char *new_value, t_gc *gc);

// -----------------------heredoc----------------------------------------
int		has_quotes(char *str);
char	*extract_var(char **src);
int		copy_char(t_expand_data *data, char c);
int		init_expand_data(t_expand_data *data);
int		validate_delimiter(const char *delimiter);
char	*expand_variables(char *line, t_env *env, int quote, t_gc *gc);
char	*replace_variable(char *line, char *var_name, char *new_val);
int		expand_variable(t_expand_data *data, char *var_name, t_env *env, t_gc *gc);
int		init_heredoc_context(t_heredoc_ctx *ctx, t_token *curr,
			t_shell *shell);
void	child_process(t_heredoc_ctx *ctx, t_shell *shell);
void	handle_parent_process(pid_t pid, t_shell *shell);
//-----------------------signaling---------------------------------------
void	ft_init_signals(void);
void	ft_set_child_signals(void);
void	ft_sigint_handler(int num);
void	ft_sigquit_handler(int num);
void	ft_set_heredoc_signals(void);
void	ft_restore_main_signals(void);
void	ft_restore_parent_signals(void);
void	ft_heredoc_sigint_handler(int signum);
// ----------------------parsing-----------------------------------------
int		ft_is_builtin(char *cmd, t_shell *shell);
int		count_total_args(t_token *tk, t_gc *gc);
int		contains_symbols(char *tk, int flag);
char	**build_argv_from_tokens(t_token *tk, t_gc *gc);
void	init_shell(t_shell *shell, char **envp);
void	after_parsing(t_shell *shell, char *input);
int		handle_empty_cmd(t_shell *shell, char *cmd);
void	handle_path_command(t_shell *shell, char *cmd);
void	child_exec(t_shell *shell, char *cmd, char **argv);
int		handle_semicolon_error(t_shell *shell, t_token *curr);
int		add_token_to_argv(char **argv, int *i, t_token *curr, t_gc *gc);
void	validate_executable(char *cmd, char *path, t_shell *shell);
int		fill_argv_from_tokens(char **argv, t_token *tk, int count, t_gc *gc);
int		add_split_parts(char **argv, int *i, char **split, int count, t_gc *gc);
void	parent_wait_and_cleanup(t_shell *shell, pid_t pid, char **argv);

// ----------------------Piping and Redirections-------------------------
int		valid_pipe(t_shell *shell, char *input);
int		handle_dless(char *delimiter, t_shell *shell, int quote);
int		handle_redirection(t_token *tk, t_shell *shell, char *input);

//-----------------------Redirection_utils--------------------------------
int		symbols(char *tk);
int		ft_index(char *str, char c);
int		count_tokens(t_token *tokens);
t_token	*filter_cmd_tokens(t_token *tk, t_gc *gc);
int		check_redirect_syntax(t_shell *shell);
int		open_last_input_redir(t_shell *shell);
void	print_cmd_error(char *cmd, char *msg);
void	execute_external_cmd(t_token *cmd_tokens,
			char **ft_env, t_shell *shell);
char	**build_args_array(t_token *cmd_tokens, t_gc *gc);
int		handle_less(char *filename, t_shell *shell);
int		open_file_input(t_token *curr, t_shell *shell);
int		open_output_file(t_token *curr, t_shell *shell);
void	handle_path_error(t_token *cmd, t_shell *shell);
void	handle_no_such_file(t_token *cmd, t_shell *shell);
int		handle_input_redirect(t_token *curr, t_shell *shell);
void	handle_directory_error(t_token *cmd, t_shell *shell);
void	handle_permission_error(t_token *cmd, t_shell *shell);
void	exec_filtered_cmd(t_shell *shell, t_token *cmd_tokens);
int		handle_standalone_less(char *filename, t_shell *shell);

//----------------------------Heredoc--------------------------------------
char	*get_delimeter(t_token *tk, t_gc *gc);
void	handle_heredoc(t_shell *shell);
int		contain_list(char *str, t_token *tk);
int		validate_delimiter(const char *delimiter);
char	*cut_from_op(char op, char *str, t_env *env, t_gc *gc);
void	handle_pipe(t_token *lst, t_shell *shell, char *input);
char	*ft_strsub(char const *s, unsigned int start, size_t len);
char	*find_command_around_heredoc(t_token *tk, char *delimiter);
char	*extract_content_without_quotes(char *str, char quote_type);
char	*replace_variable(char *line, char *var_name, char *new_val);

#endif
