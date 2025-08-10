/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 02:15:00 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/07/27 02:15:00 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEREDOC_H
# define FT_HEREDOC_H

# include "../includes/minihell.h"

typedef struct s_expand_vars
{
	char	**dst;
	int		*result_len;
	int		*buffer_size;
	char	**result;
}	t_expand_vars;

typedef struct s_expand_data
{
	char	**src;
	char	**dst;
	int		*result_len;
	int		*buffer_size;
	char	**result;
}	t_expand_data;

typedef struct s_heredoc_data
{
	char	*delimiter;
	t_shell	*shell;
	int		quote;
	int		*pipefd;
	size_t	*total_written;
}	t_heredoc_data;

typedef struct s_heredoc_params
{
	t_token	*curr;
	char	**delimiter;
	char	**cmd;
	int		*quote;
	t_shell	*shell;
}	t_heredoc_params;

typedef struct s_expansion_params
{
	char	**src;
	t_env	*env;
	char	*dst;
	int		*result_len;
	int		*buffer_size;
	char	**result;
}	t_expansion_params;

typedef struct s_char_expansion_params
{
	char	**src;
	char	**dst;
	int		*result_len;
	int		*buffer_size;
	char	**result;
}	t_char_expansion_params;

// Function declarations
char	*find_env_value(char *var_name, t_env *env);
char	*expand_variables(char *line, t_env *env, int quote);
int		handle_dless(char *delimiter, t_shell *shell, int quote);
int		count_var_chars(char *str, int start);
int		has_quotes(char *str);
void	handle_heredoc(t_shell *shell);
void	ft_handle_heredoc_child(t_shell *shell,
			char *delimiter, char *cmd, int quote);
void	ft_handle_heredoc_parent(t_shell *shell, pid_t pid, int status);
char	*ft_realloc_buffer(char *old_buf,
			int new_size, char **dst, int result_len);
int		ft_check_heredoc_size(size_t total_written,
			char *expanded_line, int *pipefd);
void	ft_write_heredoc_line(int fd,
			char *expanded_line, size_t *total_written);
char	*ft_handle_dollar_expansion(t_expansion_params *params);
char	*ft_expand_dollar_var(char **src, t_env *env, t_expand_vars *vars);
char	*ft_init_expansion_buffer(char *line, char **src, char **dst);
int		ft_expand_regular_char(t_expand_data *data);
int		ft_handle_regular_char_expansion(t_char_expansion_params *params);
int		ft_process_heredoc_line(char *line, t_heredoc_data *data);
int		ft_setup_heredoc_params(t_heredoc_params *params);
char	*ft_process_expansion_loop(t_expansion_params *params);
int		ft_setup_heredoc_pipe(char *delimiter, int *pipefd);
int		ft_fork_and_execute_heredoc(t_shell *shell, char *delimiter,
			char *cmd, int quote);
char	*ft_setup_and_process_expansion(char *src, t_env *env,
			char *dst, char *result);

#endif
