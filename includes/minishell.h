#ifndef MINIHELL_H
# define MINISHELL_H




# include "ft_printf/ft_printf.h"
# include "libft/libft.h"
# include "token.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>
# include <unistd.h>

typedef struct s_env
{
	char *line;

	struct s_env	*next;
} t_env;

typedef struct s_shell
{
    t_env *env;
    char    *path;
    t_token *tokens;
    
	int exit_status;

} t_shell;


typedef struct s_command
{
    char **args;
    char *infile;
    char *outfile;
    int append;
    struct s_command *next;
} t_command;

// error handling
void	error_print(char *str, int error_nb);
void	error_print_free(char *str, int error_nb, char **cmd);
void	ft_free_tab(char **tab);

//transform envp from struct to **
char    **transform(t_env *env);



//tokenization
void	display_list(t_token *list);

//main utils
void	ft_read(char	*input, char **ftenv);
int ft_strcmp(char *s1, char *s2);



//command
int ft_is_builtin(char *cmd);
void exec(char *cmd, char *envp[]);
char	*my_getenv(char *name, char **env);
char	*get_path(char *cmd, char *envp[]);


//environment
void	free_my_env(t_env *env);
t_env *create_env_node(char *envp);
t_env *initialize_env_list(char **envp);


//path
char	*get_my_path(t_env *env);
char	*path_extract(char *str, int count);


#endif