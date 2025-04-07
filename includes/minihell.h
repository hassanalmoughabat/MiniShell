/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:21:07 by hal-moug          #+#    #+#             */
/*   Updated: 2025/04/07 02:41:02 by njoudieh42       ###   ########.fr       */
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
# include <unistd.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_env
{
	char			*line;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	char	*path;
	int		exit_status;

}	t_shell;

// typedef struct s_command
// {
// 	char				**args;
// 	char				*infile;
// 	char				*outfile;
// 	int					append;
// 	struct s_command	*next;
// }	t_command;

// error handling
void	ft_free_tab(char **tab);
int		ft_error_message_quotes(char c);
void	error_print(char *str, int error_nb);
void	error_print_free(char *str, int error_nb, char **cmd);
void	error_message_export(char *input, char **key, char **value);

//transform envp from struct to **
char	**transform(t_env *env);

//main utils
void	print_env(char **ftenv);
void	display_list(t_token *tk);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *dest, const char *src);
void	display_token_list(int size, t_token *head);
int		ft_read(char	*input, char **ftenv, t_env *env);

// free functions
void	free_array(char **arr);
void	free_env_list(t_env *head);
void	free_token_list(t_token *head);

//command
void	exec(char *cmd, char *envp[]);
char	*my_getenv(char *name, char **env);
char	*get_path(char *cmd, char *envp[]);

//environment
void	free_my_env(t_env *env);
t_env	*create_env_node(char *envp);
int		ft_list_size(t_token *token);
t_env	*initialize_env_list(char **envp);

//path
char	*get_my_path(t_env *env);
char	*path_extract(char *str, int count);

//builtins
int		ft_pwd(void);
char	*ft_get_pwd(void);
void	ft_print_env(t_env *env);
int		ft_is_builtin(t_token *tk);
int		ft_find_old_pwd(char *str);
char	*ft_get_old_pwd(t_env *env);
void	ft_cd(t_token *tk, t_env *env, char **ft_env);
void	handle_path_command(char **ft_env, char *input);
void	handle_builtin(t_token *tk, char **ft_env, t_env *env, char *input);
void	update_env_value(t_env *head, const char *target,
			const char *new_value);
//builtings checking
// int	is_pipe(char *ft_value);
// int is_redirection(char *value);

//piping functions && redirection
void	handle_pipe(t_token *lst, char **ft_env, t_env *env);
void	handle_redirection(t_token *tk, char **ft_env, t_env *env);
//parsing 
int		is_a_path_command(char *cmd, char **ft_env);
void	after_parsing(t_token *tk, char **ft_env, t_env **env, char *input);
// copy of env
t_env 	*copy_env(t_env *envp);
void	print_env_list(t_env *env_list);
// env getters
char	*get_var(char *input);
char	*get_value_from_env(char *key ,t_env *env);
//node manipulation in env
void	ft_push_to_env(t_env **env, t_env *node);
void	ft_add_key_to_env(t_env **copy, char *key);
int		ft_update_env(char *key, char *value, t_env **env, t_env **copy);
void	ft_add_env(char *key, char *value, t_env **env, t_env **copy,int flag);
//unset
void	ft_unset(t_token *token, t_env **env, t_env **copy);
// expansion utils	
char	*handle_dollar(char *key, t_env *env, int flag);
char	*extract_and_expand(char *input, t_env *env);
void	handle_value(char *value, char **result, t_env *env);
// export
int		has_equal(char *input);
int		check_if_var_exist(t_env **env, char *key);
int		print_export_env( t_token *token, t_env *env);
void	ft_export(t_token *token, t_env **env, t_env **copy);
int		equal_handler_export(char *input, char **key, char **value, t_env *env);
// export validator
int		check_valid_key(char *key);
int		check_key_after_expansion(char *key);
// export quotes handlers
int		quote_type(char *str);
int		remove_added_quotes(char **value);
char	*quotes_in_env(char *value, char *key, int flag);
int		handle_export_quotes(char *temp, char *result, size_t *i, size_t *j);
// functions added
int 	ft_has_dollar(char *str);
char	*ft_strjoin_char(char *str, char c);
char	*ft_strcat(char *dest, const char *src);
char	*join_env_value(char *expanded, char *value);
int		ft_check_dollar(char *value, int index);
// extractors of values and keys
size_t	extract_key_env(char *line);
char	*extract_dollar_var(char *key, int *index);
char	*extract_quoted_substring(char *input, int *i);
char	*extract_unquoted_substring(char *input, int *i, int *flag);
// setters and getters of values and keys
int		has_equal_in_quote(char *input, char *quote);
char	*get_key(char *input , t_env *env, char *quote, int *ind);
void	set_key(char *input,char **result, char *quote, int *flag);
void	set_value(char **value, char quote, char *input, int flag);
char	*get_value(char *input, t_env *env, char quote, int flag);
int		set_key_value(char *input, char **key, char **value,t_env *env);
// echo
void	ft_echo(t_token *tk, t_env *env);
void	replace_dollar(t_token **t_list, t_env *env);
#endif