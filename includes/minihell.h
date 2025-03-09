/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:21:07 by hal-moug          #+#    #+#             */
/*   Updated: 2025/03/09 12:53:10 by hal-moug         ###   ########.fr       */
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
void	error_print(char *str, int error_nb);
void	error_print_free(char *str, int error_nb, char **cmd);
void	ft_free_tab(char **tab);

//transform envp from struct to **
char	**transform(t_env *env);

//main utils
int	ft_read(char	*input, char **ftenv, t_env *env);
int		ft_strcmp(char *s1, char *s2);

// free functions
void free_token_list(t_token *head);
void free_env_list(t_env *head);
void free_array(char **arr);


//command
void	exec(char *cmd, char *envp[]);
char	*my_getenv(char *name, char **env);
char	*get_path(char *cmd, char *envp[]);

//environment
void	free_my_env(t_env *env);
t_env	*create_env_node(char *envp);
t_env	*initialize_env_list(char **envp);

//path
char	*get_my_path(t_env *env);
char	*path_extract(char *str, int count);


//builtins
int ft_is_builtin(t_token *tk);
int	ft_pwd(void);
int	ft_find_old_pwd(char *str);
char	*ft_get_pwd(void);

char	*ft_get_old_pwd(t_env *env);

void update_env_value(t_env *head, const char *target, const char *new_value);
void handle_builtin(t_token *tk, char **ft_env, t_env *env, char *input);
void handle_path_command(char **ft_env, char *input);
void	ft_print_env(t_env *env);
void	ft_cd(t_token *tk, t_env *env, char **ft_env);

//builtings checking
int	is_pipe(char *ft_value);
int is_redirection(char *value);



//piping functions && redirection
void	handle_pipe(t_token *lst, char **ft_env, t_env * env);
void handle_redirection(t_token *tk, char **ft_env, t_env *env);



//parsing 
void	after_parsing(t_token *tk, char **ft_env, t_env *env, char *input);
int	is_a_path_command(char *cmd, char **ft_env);


#endif