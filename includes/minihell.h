/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:21:07 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/28 16:25:43 by njoudieh         ###   ########.fr       */
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
	bool			safe_quotes;
	struct s_env	*next;
}	t_env;

typedef struct s_redir
{
	char	**ft_env;
	t_env	*env;
}	t_redir;

typedef struct s_shell
{
	t_env	*env;
	char	*path;
	int		shell_level;
	bool	signint_child;
	bool	heredoc_sigint;
}	t_shell;

typedef enum error_msg
{
	ERROR,
	ERROR_MESG_CMD_NOT_FOUND,
	ERROR_MESG_NO_FILE,
	ERROR_MESG_PERMISSION_DENIED,
	ERROR_MESG_TOO_MANY_ARGS,
	ERROR_MESG_NUMERIC_REQUIRED
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

typedef struct s_heredoc_info
{
	int		fd;
	t_token	*position;
}	t_heredoc_info;

typedef struct s_pipe_data
{
	t_token			*lst;
	char			**ft_env;
	t_env			*env;
	int				**pipes;
	int				pipe_count;
	t_heredoc_info	*heredocs;
	int				hd_count;
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

extern t_shell	g_minishell;

int	is_delimeter_quoted(t_token *tk);
int		is_quote_token(char *str);

// --------------value getter setter----------------------------
char	*get_value(char *input, char quote, int flag);
char	*get_key(t_token *tk, char *input, t_env *env, char *quote, int *ind);
void	set_value(char **value, char quote, char *input, int flag);
void	set_key(char *input, char **result, char *quote, int *flag);
int		set_key_value(t_token *tk, char **key, char **value, t_env *env);
// --------------------cd----------------------------------------
char	*ft_get_pwd(void);
int		ft_find_old_pwd(char *str);
char	*ft_get_old_pwd(t_env *env);
void	ft_cd(t_token *tk, t_env *env, char **ft_env);
// --------------------echo---------------------------------------
void	ft_echo(t_token *tk);
// --------------------export-------------------------------------
int		has_equal(char *input);
int		check_valid_key(t_token *tk, char *key, t_env *env);
void	ft_export(t_token *token, t_env **copy);
void	ft_add_key_to_env(t_env **copy, char *key);
int		check_if_var_exist(t_env **env, char *key);
char	*get_value_from_env(char *key, t_env *env);
int		print_export_env( t_token *token, t_env *env);
char	*quotes_in_env(char *value, char *key, int flag);
int		ft_update_env(char *key, char *value, t_env **env, t_env **copy);
int		handle_export_quotes(char *temp, char *result, size_t *i, size_t *j);
int		equal_handler_export(t_token *tk, char *input, char **key,
			char **value, t_env *env);
// ---------------------env------------------------------------------
char	*get_var(char *input);
size_t	extract_key_env(char *line);
char	*my_getenv(char *name, char **env);
void	ft_push_to_env(t_env **env, t_env *node);
void	ft_add_env(char *key, char *value, t_env **copy, int flag);
// ----------------------pwd-----------------------------------------
int		ft_pwd(t_token *tk);
// ----------------------helpers for builtins------------------------
void	print_env(char **ftenv);
void	ft_print_env(t_env *env, char **envp);
void	display_list(t_token *tk);
int		ft_list_size(t_token *token);
int		ft_strcmp(char *s1, char *s2);
void	print_env_list(t_env *env_list);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strcpy(char *dest, const char *src);
void	handle_builtin(t_token *tk, char **ft_env, t_env **env);
// ----------------------copy env-------------------------------------
t_env	*copy_env(t_env *envp);
char	**transform(t_env *env);
// ----------------------get path-------------------------------------
char	*get_my_path(t_env *env);
t_env	*create_env_node(char *envp);
t_env	*initialize_env_list(char **envp);
char	*get_path(char *cmd, char *envp[]);
char	*path_extract(char *str, int count);
// ----------------------free functions-------------------------------
void	free_array(char **arr);
void	free_my_env(t_env *env);
void	ft_free_tab(char **tab);
void	free_env_list(t_env *head);
void	free_shell(t_shell *shell);
void	free_token_list(t_token *head);
void	cleanup_minishell(t_shell *shell, char **transform_result, char *input);
// ----------------------error messages--------------------------------
int		ft_err_msg(t_error err);
int		ft_error_message_quotes(char c);
void	error_message_export(char **str);
void	error_validity_export(char *input, t_token *tk);
// ----------------------expansion-------------------------------------
int		ft_has_dollar(char *str);
void	replace_dollar(t_token **t_list);
char	*handle_dollar(char *key, int flag);
char	*ft_strjoin_char(char *str, char c);
char	*extract_value(char *str, int *index);
int		expand_helper(char **substr, int flag);
int		ft_check_dollar(char *value, int index);
void	handle_value(char *value, char **result);
int		ft_check_exceptions(char *str, int index);
char	*extract_dollar_var(char *key, int *index);
char	*join_env_value(char *expanded, char *value);
int		check_key_after_expansion(char *key, t_env *env, t_token *tk);
void	get_dollar_val(char *key, int *i, char **expanded);
int		dollar_cases(char *key, int *index, char **expanded);
// -----------------------exit------------------------------------------
void	handle_exit_code(t_token *curr);
void	ft_exit(t_token *tk, t_env **copy);
// -----------------------shell level-----------------------------------
void	decrement(t_shell *shell);
void	shell_handling(t_token *tk);
void	add_shell_level(t_env **env, char *new_line);
void	update_shlvl_in_env(t_env **env, int new_shlvl);
// -----------------------quotes----------------------------------------
int		quote_type(char *str);
int		remove_added_quotes(char **value);
int		has_equal_in_quote(char *input, char *quote);
char	*extract_quoted_substring(char *input, int *i);
char	*extract_unquoted_substring(char *input, int *i, int *flag);
// -----------------------unset------------------------------------------
void	update_env_value(t_env *head, const char *target,
		const char *new_value);
void	ft_unset(t_token *token, t_env **env, t_env **copy);
// -----------------------heredoc----------------------------------------
char	*get_delimeter(t_token *tk);
int		contain_char(char *str, char c);
int		validate_delimiter(const char *delimiter);
char	*cut_from_op(char op, char *str, t_env *env);
char	*find_command_around_heredoc(t_token *tk, char *delimiter);
char	*replace_variable(char *line, char *var_name, char *new_val);
//-----------------------signaling---------------------------------------
void	ft_init_signals(void);
void	ft_sigint_handler(int num);
void	ft_sigquit_handler(int num);
// ----------------------parsing-----------------------------------------
int		ft_is_builtin(char *cmd);
int		is_a_path_command(char *cmd, char **ft_env);
void	handle_path_command(t_token *tk, char *envp[], char *cmd);
void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input);
// ----------------------Piping and Redirections-------------------------
int		valid_pipe(t_token *tk);
int		redirect_count(t_token *tk);
void	handle_pipe(t_token *lst, char **ft_env, t_env *env, char *input);
int		handle_several_redirection(t_token *tk, char **ft_env);
int		handle_redirection(t_token *tk, char **ft_env, t_env *env);
int		handle_dless(char *delimiter, t_env *env, int flag, int quote);
int		handle_great(char *filename, t_token *tk, char **ft_env, t_env *env);
int		handle_dgreat(char *filename, t_token *tk, char **ft_env, t_env *env);
//-----------------------Redirection_utils--------------------------------
int		count_tokens(t_token *tokens);
void	print_cmd_error(char *cmd, char *msg);
char	**build_args_array(t_token *cmd_tokens);
int		open_output_file(char *filename, int flags);
t_redir	*init_redir_params(char **ft_env, t_env *env);
t_token	*find_redirect_token(t_token *tk, char *redirect_op);
void	execute_external_cmd(t_token *cmd_tokens, char **ft_env);
int		handle_standalone_redirect(char *filename, t_token *tk, int flags);
t_token	*copy_tokens_before_redirect(t_token *tk, t_token *redirect_token);
void	execute_builtin_redirect(t_token *cmd_tokens, char **ft_env,
		t_env *env);
void	execute_with_redirect(t_token *cmd_tokens, char **ft_env,
		t_env *env, int fd);
int		handle_redirect_child(int fd, t_token *tk, t_token *redirect_token,
		t_redir *params);
int		handle_redirect_fork(int fd, t_token *tk, t_token *redirect_token,
		t_redir *params);
//---------------------Handle redirection utils----------------------------
int		handle_output_redirect(t_token *curr, t_token *tk,
		char **ft_env);
int		handle_input_redirect(t_token *curr, t_token *tk,
		char **ft_env, t_env *env);	
int		handle_less(char *filename, t_token *tk, char **ft_env, t_env *env);
//----------------------------Heredoc--------------------------------------
char	*get_delimeter(t_token *tk);
int		contain_char(char *str, char c);
char	*extract_variable(const char *line);
int		contain_list(char *str, t_token *tk);
int		validate_delimiter(const char *delimiter);
char	*cut_from_op(char op, char *str, t_env *env);
char	*ft_strsub(char const *s, unsigned int start, size_t len);
char	*find_command_around_heredoc(t_token *tk, char *delimiter);
void	handle_heredoc(char **ft_env, t_env *env, t_token *tk);
char	*extract_content_without_quotes(char *str, char quote_type);
char	*replace_variable(char *line, char *var_name, char *new_val);

#endif
