/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:48:25 by njoudieh          #+#    #+#             */
/*   Updated: 2025/06/02 18:54:22 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/wait.h>
# include "../includes/libft/libft.h"

typedef enum s_token_type
{
	T_IDENTIFIER,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_PIPE,
	T_NL,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*cmd;
	int 			processed;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;
int		ft_check_space(char c);
int		ft_delimeter(char *str);
int		ft_check_quotes(char c);
t_token	*ft_tokenize(char *input);
t_token	*ft_check_string(char *input);
int		ft_error_message_quotes(char c);
int		ft_check_special_char(char *str);
void	ft_skip_added_spaces(char **str);
bool	escape(const char *str, int index);
void	ft_free_token_list(t_token **list);
int		ft_find_next_quote(char *line, size_t *i);
int		ft_handle_operators(char **input, t_token **list);
t_token	*create_token(char *value, t_token_type operator);
int		ft_handle_quotes(char **input, t_token **token_list);
void	ft_add_token_last(t_token **t_list, t_token *new_token);
int		ft_append_operator(t_token **list, t_token_type operator,
			char **input, char *value);
#endif