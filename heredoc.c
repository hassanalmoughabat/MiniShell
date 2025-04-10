/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:35:40 by hal-moug          #+#    #+#             */
/*   Updated: 2025/04/10 20:44:29 by hal-moug         ###   ########.fr       */
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
    
    // Check if string starts and ends with the same quote type
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

int handle_dless(char *delimiter, t_env *env, int flag)
{
    int pipefd[2];
    size_t total_written = 0;
    char *line;
    char *val;
	char *oldval;

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
        
        if (contain_char(line, '$') && flag == 1)
        {
            val = cut_from_op('$', line, env); 
			oldval =  extract_variable(line);
			line = replace_variable(line, oldval, val);

        }
        size_t line_len = strlen(line);
        if (total_written + line_len + 1 > MAX_HEREDOC_SIZE)
        {
            free(line);
            close(pipefd[0]);
            close(pipefd[1]);
            return -1;
        }
        write(pipefd[1], line, line_len);
        write(pipefd[1], "\n", 1);
        total_written += line_len + 1;
        free(line);
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

    delimeter = get_delimeter(tk);
	printf("delimeter is %s\n", delimeter);
    cmd = find_command_around_heredoc(tk, delimeter);
	if (cmd)
    {
		char *args[] = {cmd, NULL};
	    path = get_path(cmd, ft_env);
		curr = tk;
	    read_fd = handle_dless(delimeter, env, 1);
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
		handle_dless(delimeter, env, 0);
}