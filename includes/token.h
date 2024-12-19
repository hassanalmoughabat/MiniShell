/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:48:25 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/19 15:04:00 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"

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
	int				quotes;
	char			*cmd;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

t_token	ft_tokenize(char *input);

void	ft_add_token_last(t_token **t_list, t_token *new_token);
void	ft_check_string(char *input, t_token **token_list);
void	ft_skip_added_spaces(char **str);
void	ft_free_token_list(t_token **list);

int		create_token(t_token **t_list, t_type_token operator);
int		ft_handle_operators(char *input, t_token **list);
int		check_unique_char(char s, char before);
int		ft_check_special_char(char *str);
int		ft_check_quotes(char *str);
int		ft_check_space(char c);

#endif