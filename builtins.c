/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:17:06 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/04/03 00:23:27 by njoudieh42       ###   ########.fr       */
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

size_t my_strnlen(const char *s, size_t maxlen)
{
    size_t len = 0;

    while (len < maxlen && s[len])
        len++;
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
	if (len >= 2 && ft_check_quotes(str[0]) && ft_check_quotes(str[len - 1]))
	{
		trimmed = my_strndup(str + 1, len - 2);
		free(str);
		return (trimmed);
	}
	return (str); 
}

char *remove_inner_quotes(char *str)
{
    size_t len = ft_strlen(str);

    char *result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    size_t i = 0, j = 0;
    while (i < len)
	{  
        if (str[i] == '\\' && (ft_check_quotes(str[i + 1])))
		{
			i ++;
			result[j++] = str[i];
		}
		else if (str[i] == '\\' && str[i + 1] == '$')
		{
			i ++;
			result[j++] = str[i];
		}
		else if (str[i] == '\\' && str[i + 1] == '\\')
			result[j++] = str[i++];
		else if (str[i] != '"' && str[i] != '\'')
        	result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return result;
}

bool has_inner_quotes(char *str)
{
    size_t len;
    size_t i;

	i = 0;
    if (!str)
        return false;
    len = ft_strlen(str);
    while (i < len) 
	{
        if ((str[i] == '"' || str[i] == '\'' || str[i] == '\\' || str[i] == '$') && escape(str, i))
			return true;
        i++;
    }
    return false;
}

void	ft_echo(t_token *tk, t_env *env)
{
	t_token	*curr;
	int		flag;
	char	*temp;

	flag = 0;
	curr = tk;
	temp = NULL;
	if (curr && !ft_strcmp(curr->cmd, "echo"))
		curr = curr->next;
	while (curr)
	{
		if (!ft_strcmp(curr->cmd, "-n") || !ft_strcmp(curr->cmd, "\"-n\""))
		{
			flag = 1;
			curr = curr->next;
			continue ;
		}
		if (curr)
		{
			handle_value(curr->cmd, &temp, env);
			if (temp)
				ft_printf("%s ",temp);
		}
		curr = curr->next;
	}
	if (!flag)
		ft_printf("\n");
}

void	handle_builtin(t_token *tk, char **ft_env, t_env *env, char *input)
{
	static 	t_env	*copy;

	copy = NULL;
	if (copy == NULL)
		copy = copy_env(env);
	if (!ft_strcmp(tk->cmd, "env"))
		ft_print_env(env);
	else if (!ft_strcmp(tk->cmd, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(tk->cmd, "cd"))
		ft_cd(tk, env, ft_env);
	else if (!ft_strcmp(tk->cmd, "exit"))
		ft_exit(tk, ft_env, env, input);
	else if (!ft_strcmp(tk->cmd, "export"))
		ft_export(tk, &env,&copy);
	else if (!ft_strcmp(tk->cmd, "unset"))
		ft_unset(tk, &env, &copy);
	else if (!ft_strcmp(tk->cmd, "echo"))
		ft_echo(tk, env);
}
		
