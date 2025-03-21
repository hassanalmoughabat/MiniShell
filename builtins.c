/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/03/21 21:29:22 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minihell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (1);
	}
	else
		return (0);
}

void	ft_print_env(t_env *env)
{
	t_env	*ev;

	ev = env;
	while (ev)
	{
		ft_printf("%s\n", ev->line);
		ev = ev->next;
	}
}

void	ft_exit(t_token *tk, char **ft_env, t_env *env, char *input)
{
	int		exit_code;
	t_token	*curr;

	curr = tk->next;
	exit_code = 0;
	ft_printf("exit\n");
	if (curr && curr->cmd)
		exit_code = ft_atoi(curr->cmd);
	free_token_list(tk);
	free_env_list(env);
	free_array(ft_env);
	free(input);
	exit(exit_code % 256);
}
void	handle_dollar(char *str, t_env *env)
{
	char	*env_value;

	env_value = my_getenv(str + 1, transform(env));
	if (env_value && str)
		ft_printf("%s", env_value);
	else
		ft_printf("Variable not found\n");
}

size_t my_strnlen(const char *s, size_t maxlen) {
    size_t len = 0;

    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
}

char *my_strndup(const char *s, size_t n)
 {
    size_t len = my_strnlen(s, n);
    char *new_str = (char *)malloc(len + 1);

    if (!new_str)
        return NULL;

    ft_memcpy(new_str, s, len);
    new_str[len] = '\0';

    return new_str;
}

char	*trim_outer_quotes(char *str)
{
	size_t	len;
	char	*trimmed;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ft_check_quotes(str[0]) && ft_check_quotes(str[len -1]))
	{
		trimmed = my_strndup(str + 1, len - 2);
		free(str);
		return (trimmed);
	}
	ft_printf("JRSF\n");
	return (str); 
}

char *remove_inner_quotes(char *str)
{
    size_t len = ft_strlen(str);

    char *result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    size_t i = 1, j = 0;  
    result[j++] = str[0];
    while (i < len - 1) {  
        if (str[i] != '"' && str[i] != '\'')
            result[j++] = str[i];
        i++;
    }
    result[j++] = str[len - 1];
    result[j] = '\0';
    return result;
}

bool has_inner_quotes(char *str)
{
    size_t len;
    size_t i;

	i = 1;
    if (!str)
        return false;
    len = ft_strlen(str);
    while (i < len - 1) 
	{
        if (str[i] == '"' || str[i] == '\'')
            return true;
        i++;
    }
    return false;
}

void	ft_echo(t_token *tk, t_env *env)
{
	t_token	*curr;
	int		flag;
	int		quotes;
	size_t	len;
	char	*temp;

	flag = 0;
	quotes = 0;
	curr = tk;
	
	while (curr)
	{
		
		if (ft_strcmp(curr->cmd, "echo") == 0)
			curr = curr->next;
		if (ft_strcmp(curr->cmd, "-n") == 0)
		{
			flag = 1;
			curr = curr->next;
			continue ;
		}
		len = ft_strlen(curr->cmd);
		if (ft_check_quotes(curr->cmd[0]) && ft_check_quotes(curr->cmd[len -1]))
		{
			temp = trim_outer_quotes(curr->cmd);
			quotes= 1;
			ft_printf("esdjfz\n");
		}
		else if (has_inner_quotes(curr->cmd) && !quotes)
			temp =remove_inner_quotes(curr->cmd);
		else 
			temp = ft_strdup(curr->cmd);
		temp = replace_dollars(temp, env);
		if (temp)
			ft_printf("%s ",temp);
		curr = curr->next;
	}
	if (!flag)
		ft_printf("\n");
}

void	handle_builtin(t_token *tk, char **ft_env, t_env *env, char *input)
{
	t_token	*curr;

	curr = tk;
	while (curr)
	{
		if (ft_strcmp(curr->cmd, "env") == 0)
			ft_print_env(env);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			ft_pwd();
		else if (ft_strcmp(curr->cmd, "cd") == 0)
			ft_cd(tk, env, ft_env);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			ft_exit(tk, ft_env, env, input);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			ft_export(tk, env);
		else if (ft_strcmp(curr->cmd, "unset") == 0)
			ft_unset(tk, &env);
		else if (ft_strcmp(curr->cmd, "echo") == 0)
			ft_echo(tk, env);
		curr = curr->next;
	}
}
