/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:48:25 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/24 21:49:49 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

// # include "minishell.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include "../ft_libft/libft.h"
// #include "token.h"
typedef enum s_token_type
{
	T_IDENTIFIER,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_PIPE,
	T_Squote,
	T_Dquote,
	T_NL,
}	t_type_token;

typedef struct s_token
{
	t_type_token	type;
	char			*cmd;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

t_token	ft_tokenize(char *input);
t_token	*create_token(char *value, t_type_token operator);


void	ft_add_token_last(t_token **t_list, t_token *new_token);
void	ft_check_string(char *input, t_token **token_list);
void	ft_skip_added_spaces(char **str);
void	ft_free_token_list(t_token **list);
void	display_list(t_token *list);

int		ft_handle_operators(char *input, t_token **list);
int		ft_handle_quotes(char **input, t_token **list);
int		ft_append_operator(t_token **list, t_type_token operator, char **input);
int		ft_check_special_char(char *str);
int		ft_check_space(char c);
int		ft_check_type_quote(char c);

char	*ft_trim_quotes(char *str, int i, int j);
char	*ft_handle_single_quotes(char **input, int **i, int **j);
char	*ft_handle_double_quotes(char **input, int **i, int **j);
char	*ft_handle_word(char **input, int **i, int **j);


#endif