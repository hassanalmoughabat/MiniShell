/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:25:06 by hassan-al-m       #+#    #+#             */
/*   Updated: 2024/12/19 17:29:29 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

#define MINISHELL_H

#define HISTFILE ".minihell_history"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h> // For fork() and execve()
# include <stdio.h>  // For perror()
# include <stdlib.h> // For exit()

void	exec(char *cmd, char *envp[]);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);

void init_shell(void);
void cleanup_shell(void);

int is_builtin(char *cmd);
int handle_builtin(char **args, char **envp);
void exec(char *cmd, char *envp[]);

char	*ft_strjoin(char const *s1, char const *s2);

char	*ft_substr(char const *s, unsigned int start, size_t len);

char	**ft_split(char const *s, char c);
char	*get_path(char *cmd, char *envp[]);
char	*my_getenv(char *name, char **env);

#endif