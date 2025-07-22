/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:40:13 by hal-moug          #+#    #+#             */
/*   Updated: 2025/07/22 20:14:55 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

int	validate_delimiter(const char *delimiter)
{
	if (!delimiter || *delimiter == '\0')
		return (0);
	return (1);
}

// int contain_char(char *str, char c)
// {
//     int i;

//     i = 0;
//     while (str[i])
//     {
//         if (str[i] == c)
//             return (1);
//         i++;
//     }
//     return (0);
// }

int ft_index(char *str, char c)
{
	int i = 0;

	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

char *cut_from_op(char op, char *str, t_env *env)
{
    int pos;
    int count = 0;
    int start;
    int end;
    char *value;
    char *send;
    int i ;

    pos = ft_index(str, op); 
    start = pos;
    count = 0;
    while(str[start] && str[++start] != ' ' && str[start] != '\t')
        count++;
    end = start;
    value = (char *)malloc(sizeof(char) * (count + 1));
    if (!value)
        return NULL;
    start = pos + 1;
    i = 0; 
    while (str[start] && start < end)
        value[i++] = str[start++];
    value[i] = '\0';  
    send = my_getenv(value, transform(env));
    return send;
}

 char *replace_variable(char *line, char *var_name, char *new_val)
{
    size_t before_len;
    size_t var_name_len;
    size_t new_val_len;
    size_t after_len;
    size_t new_len;
    char *pos;

    pos = strstr(line, var_name);
    if (!pos)
        return ft_strdup(line); 
    before_len = pos - line;
    var_name_len = ft_strlen(var_name);
    new_val_len = ft_strlen(new_val);
    after_len = ft_strlen(pos + var_name_len);
    new_len = before_len + new_val_len + after_len;
    char *new_line = malloc(new_len + 1);
    if (!new_line)
    {
           return (NULL);
    }
    strncpy(new_line, line, before_len);                   
    ft_strcpy(new_line + before_len, new_val);                 
    ft_strcpy(new_line + before_len + new_val_len, pos + var_name_len);
    return (new_line);
}

// char *extract_variable(const char *line)
// {
//     char prefix;
//     char *var_start;
//     char *var_end;
//     size_t var_len;
//     char *variable;

//     prefix = '$';
//     var_start = strchr(line, prefix);
//     if (!var_start) {
//         return NULL;
//     }
//     var_end = var_start + 1; 
//     while (*var_end && (isalnum(*var_end) || *var_end == '_')) {
//         var_end++;
//     }
//     var_len = var_end - var_start;
//     variable = malloc(var_len + 1);
//     if (!variable) {
//         return NULL;
//     }
//     strncpy(variable, var_start, var_len);
//     variable[var_len] = '\0'; 
//     return variable;
// }
