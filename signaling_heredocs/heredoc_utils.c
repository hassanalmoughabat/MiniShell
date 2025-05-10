/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:33:08 by njoudieh42        #+#    #+#             */
/*   Updated: 2025/05/02 18:06:26 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minihell.h"

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

int handle_dless(char *delimiter, t_env *env, int flag, int quote)
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
        if (quote == 0)
      	  if (contain_char(line, '$') && flag == 1  && !contain_char(line, '\"'))
       	 	{
				if (contain_char(line, '\'') == 1)
					line = extract_variable_quote(line);
        	    val = cut_from_op('$', line, env); 
				oldval =  extract_variable_quote(line);
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
