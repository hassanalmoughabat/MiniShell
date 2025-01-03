#ifndef TOKEN_H
# define TOKEN_H

typedef enum s_token_type
{
	T_IDENTIFIER,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_PIPE,
}	t_token_type;

typedef struct s_token
{
	t_token_type type;
	char	*cmd;
	struct s_token *prev;
	struct s_token *next;
}	t_token;


void	display_list(t_token *list);



#endif