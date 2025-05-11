/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:35:40 by hal-moug          #+#    #+#             */
/*   Updated: 2025/05/11 12:40:09 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"



char *extract_content_without_quotes(char *str, char quote_type)
{
    int     i;
    int     j;
    int     len;
    char    *result;

    if (!str)
        return (NULL);
    
    len = ft_strlen(str);
    if (len < 2)
        return (ft_strdup(str));
        if (str[0] == quote_type && str[len - 1] == quote_type)
    {
        result = (char *)malloc(sizeof(char) * (len - 1));
        if (!result)
            return (NULL);
        
        i = 1;
        j = 0;
        while (i < len - 1)
            result[j++] = str[i++];
        result[j] = '\0';
        return (result);
    }
    return (ft_strdup(str));
}

char *ft_strsub(char const *s, unsigned int start, size_t len)
{
    char    *str;
    size_t  i;

    if (!s)
        return (NULL);
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        str[i] = s[start + i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

char *get_delimeter(t_token *tk)
{
    t_token *curr;
    char    *delimeter;
    int     len;

    curr = tk;
    while (curr)
    {
        if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            delimeter = curr->next->cmd;
            len = ft_strlen(delimeter);
            if (len >= 2 && delimeter[0] == '\'' && delimeter[len - 1] == '\'')
                return (ft_strsub(delimeter, 1, len - 2));
            else if (len >= 2 && delimeter[0] == '\"' && delimeter[len - 1] == '\"')
                return (ft_strsub(delimeter, 1, len - 2));
            else
                return (ft_strdup(delimeter));

        }
        curr = curr->next;
    }
    return (NULL);
}
char *find_command_around_heredoc(t_token *tk, char *delimiter)
{
    t_token *curr = tk;
    t_token *command_before = NULL;
    
    while (curr)
    {
        if (curr->next && ft_strcmp(curr->next->cmd, "<<") == 0)
            command_before = curr;

        if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            curr = curr->next;
            while (curr)
            {
                if (ft_strcmp(curr->cmd, delimiter) == 0 && curr->next)
                    return curr->next->cmd;
                curr = curr->next;
            }
            if (command_before)
                return command_before->cmd;
                
            return NULL;
        }
        curr = curr->next;
    }
    return NULL;  
}

static int is_delimeter_quoted(t_token *tk)
{
    t_token *curr;
    char    *delimeter;
    int     i;

    curr = tk;
    while (curr)
    {
        if (ft_strcmp(curr->cmd, "<<") == 0 && curr->next)
        {
            delimeter = curr->next->cmd;
            i = 0;
            while (delimeter[i])
            {
                if (delimeter[i] == '\'' || delimeter[i] == '\"')
                    return (1);
                i++;
            }
        }
        curr = curr->next;
    }
    return (0);
}

char *extract_variable_quote(char *line)
{
    if (!line)
        return NULL;
    size_t start = 0;
    size_t end = strlen(line);
    while (line[start] == '\'' && start < end)
        start++;
    while (end > start && line[end - 1] == '\'')
        end--;
    size_t len = end - start;
    if (len == 0)
        return NULL;
    char *result = (char *)malloc(len + 1);
    if (!result)
        return NULL;
    strncpy(result, line + start, len);
    result[len] = '\0';

    return result;
}

static int is_quoted_format(const char *line)
{
    if (!line)
        return 0;
    size_t len = strlen(line);
    if (len >= 4 && line[0] == '\'' && line[1] == '\'' && line[len - 2] == '\'' && line[len - 1] == '\'')
        return 1;
    return 0;
}

int handle_dless(char *delimiter, t_env *env, int flag, int quote)
{
    int pipefd[2];
    size_t total_written = 0;
    char *line;
    char *val;
    char *oldval;
    char *processed_line;

    if (!validate_delimiter(delimiter))
        return -1;
    if (pipe(pipefd) == -1)
        return -1;
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        processed_line = NULL;
        if (quote == 0 && contain_char(line, '$') && flag == 1 && !contain_char(line, '\"'))
        {
            if (is_quoted_format(line))
            {
                char *inner_content = ft_strsub(line, 2, strlen(line) - 4);
                oldval = extract_variable(inner_content);
                if (oldval)
                {
                    val = cut_from_op('$', inner_content, env);
                    if (val)
                    {
                        processed_line = malloc(strlen(val) + 3);
                        if (processed_line)
                        {
                            processed_line[0] = '\'';
                            strcpy(processed_line + 1, val);
                            processed_line[strlen(val) + 1] = '\'';
                            processed_line[strlen(val) + 2] = '\0';
                        }
                        free(inner_content);
                    }
                    free(oldval);
                }
                if (!processed_line)
                    processed_line = ft_strdup(line);
            }
            else if (line[0] == '\'' && line[strlen(line) - 1] == '\'')
            {
                char *inner_content = ft_strsub(line, 1, strlen(line) - 2);
                oldval = extract_variable(inner_content);
                if (oldval)
                {
                    val = cut_from_op('$', inner_content, env);
                    processed_line = ft_strdup(val ? val : "");
                    free(oldval);
                }
                free(inner_content);
                if (!processed_line)
                    processed_line = ft_strdup(line);
            }
            else
            {
                oldval = extract_variable(line);
                if (oldval)
                {
                    val = cut_from_op('$', line, env);
                    if (val)
                    {
                        processed_line = replace_variable(line, oldval, val);
                        free(oldval);
                    }
                }
            }
        }
        char *final_line;
        if (processed_line)
            final_line = processed_line;
        else
            final_line = line;
        size_t line_len = strlen(final_line);

        if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
        {
            free(line);
            if (processed_line)
                free(processed_line);
            close(pipefd[0]);
            close(pipefd[1]);
            return -1;
        }
        write(pipefd[1], final_line, line_len);
        write(pipefd[1], "\n", 1);
        total_written += line_len + 1;
        free(line);
        if (processed_line)
            free(processed_line);
    }
    close(pipefd[1]);
    return pipefd[0];
}

 void handle_cat_heredoc(char **ft_env, t_env *env, t_token *tk)
{
    t_token *curr;
    int read_fd;
    pid_t pid;
    int status;
    char *path;
	char *cmd;
    char *delimeter;
	int quote;

	quote = is_delimeter_quoted(tk);
    delimeter = get_delimeter(tk);
	printf("delimeter is %s\n", delimeter);
    cmd = find_command_around_heredoc(tk, delimeter);
	if (cmd)
    {
		char *args[] = {cmd, NULL};
	    path = get_path(cmd, ft_env);
		curr = tk;
	    read_fd = handle_dless(delimeter, env, 1, quote);
	    if (!read_fd)
		        return;
   		 pid = fork();  
    	if (pid == -1)
    	    return;
   		 else if (pid == 0)
    	{
        	dup2(read_fd, STDIN_FILENO);
        	close(read_fd);
       		 execve(path, args, ft_env);
       		 exit(EXIT_FAILURE);
    	}
    	else
    	{
   			  close(read_fd);
   	    	 waitpid(pid, &status, 0);
   	 	}
	}
	else
		handle_dless(delimeter, env, 0, quote);
}
