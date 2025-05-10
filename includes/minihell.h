/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:21:07 by hal-moug          #+#    #+#             */
/*   Updated: 2025/05/11 01:43:26 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHELL_H
# define MINIHELL_H

# include "ft_printf/ft_printf.h"
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../tokenize/token.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <signal.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# define MAX_HEREDOC_SIZE 10000	

typedef struct s_env
{
	char			*line;
	int				exit_status;
	struct s_env	*next;
}	t_env;

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

extern t_shell	g_minishell;

// typedef struct s_command
// {
// 	char				**args;
// 	char				*infile;
// 	char				*outfile;
// 	int					append;
// 	struct s_command	*next;
// }	t_command;

// --------------value getter setter----------------------------
char	*get_key(char *input, t_env *env, char *quote, int *ind);
char	*get_value(char *input, char quote, int flag);
void	set_value(char **value, char quote, char *input, int flag);
void	set_key(char *input, char **result, char *quote, int *flag);
int		set_key_value(char *input, char **key, char **value, t_env *env);
// --------------------cd----------------------------------------
char	*ft_get_pwd(void);
int		ft_find_old_pwd(char *str);
char	*ft_get_old_pwd(t_env *env);
void	ft_cd(t_token *tk, t_env *env, char **ft_env);
// --------------------echo---------------------------------------
void	ft_echo(t_token *tk);
// --------------------export-------------------------------------
int		has_equal(char *input);
int		check_valid_key(char *key, t_env *env);
int		check_valid_key(char *key, t_env *env);
void	ft_add_key_to_env(t_env **copy, char *key);
int		check_if_var_exist(t_env **env, char *key);
char	*get_value_from_env(char *key, t_env *env);
int		print_export_env( t_token *token, t_env *env);
int		check_key_after_expansion(char *key, t_env *env);
char	*quotes_in_env(char *value, char *key, int flag);
void	ft_export(t_token *token, t_env **copy);
int		ft_update_env(char *key, char *value, t_env **env, t_env **copy);
int		handle_export_quotes(char *temp, char *result, size_t *i, size_t *j);
int		equal_handler_export(char *input, char **key, char **value, t_env *env);
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
void	ft_print_env(t_env *env);
void	display_list(t_token *tk);
int		ft_list_size(t_token *token);
int		ft_strcmp(char *s1, char *s2);
void	print_env_list(t_env *env_list);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strcpy(char *dest, const char *src);
void	handle_builtin(t_token *tk, char **ft_env, t_env **env);
// ----------------------copy env-------------------------------------
char	**transform(t_env *env);
t_env	*copy_env(t_env *envp);
// ----------------------get path-------------------------------------
char	*get_my_path(t_env *env);
t_env	*create_env_node(char *envp);
t_env	*initialize_env_list(char **envp);
char	*get_path(char *cmd, char *envp[]);
char	*path_extract(char *str, int count);
// ----------------------free functions-------------------------------
void	free_my_env(t_env *env);
void	free_array(char **arr);
void	ft_free_tab(char **tab);
void	free_env_list(t_env *head);
void	free_shell(t_shell *shell);
void	free_token_list(t_token *head);
// ----------------------error messages--------------------------------
int		ft_err_msg(t_error err);
int		ft_error_message_quotes(char c);
void	error_validity_export(char *input);
void	error_message_export(char **str);
// ----------------------expansion-------------------------------------
int		ft_has_dollar(char *str);
char	*ft_strjoin_char(char *str, char c);
char	*extract_value(char *str, int *index);
int		ft_check_dollar(char *value, int index);
int		ft_check_exceptions(char *str, int index);
char	*extract_dollar_var(char *key, int *index);
void	replace_dollar(t_token **t_list);
char	*join_env_value(char *expanded, char *value);
char	*handle_dollar(char *key, int flag);
int		check_key_after_expansion(char *key, t_env *env);
void	get_dollar_val(char *key, int *i, char **expanded);
int		expand_helper(char **substr, int flag);
void	handle_value(char *value, char **result);
int		dollar_cases(char *key, int *index, char **expanded);
// -----------------------exit------------------------------------------
void	handle_exit_code(t_token *curr);
void	ft_exit(t_token *tk, t_env **copy);
// -----------------------shell level-----------------------------------
void	decrement(t_shell *shell);
void	shell_handling(t_token *tk);
void	update_shlvl_in_env(t_env **env, int new_shlvl);
void	add_shell_level(t_env **env, char *new_line);
// -----------------------quotes----------------------------------------
int		quote_type(char *str);
int		remove_added_quotes(char **value);
int		has_equal_in_quote(char *input, char *quote);
char	*extract_quoted_substring(char *input, int *i);
char	*extract_unquoted_substring(char *input, int *i, int *flag);
// -----------------------unset------------------------------------------
void	ft_unset(t_token *token, t_env **env, t_env **copy);
void	update_env_value(t_env *head, const char *target,
			const char *new_value);
// -----------------------heredoc----------------------------------------
char	*get_delimeter(t_token *tk);
int		contain_char(char *str, char c);
int		validate_delimiter(const char *delimiter);
char	*cut_from_op(char op, char *str, t_env *env);
char	*find_command_around_heredoc(t_token *tk, char *delimiter);
char	*replace_variable(char *line, char *var_name, char *new_val);
//-----------------------signaling---------------------------------------
void	ft_init_signals(void);
void	ft_sigquit_handler(int num);
void	ft_sigint_handler(int num);
// ----------------------parsing-----------------------------------------
int		ft_is_builtin(t_token *tk);
int		is_a_path_command(char *cmd, char **ft_env);
void	handle_path_command(t_token *tk, char *envp[], char *cmd);
void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input);
#endif