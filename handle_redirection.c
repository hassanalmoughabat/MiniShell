#include "includes/minihell.h"

int handle_dgreat(char *filename)
{

    int fd;

    if (!filename || *filename == '\0')
    {
        printf("the filename is empty");
        return 0;
    }
    fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (!fd)
    {
        printf("error creating the file");
        return (0);
    }
    return (1);
    /* it crates a file if it doesn't exist
    it preserves what was in file before and appends to end of it*/
}
int handle_great(char *filename)
{
    int fd;

    if (!filename || *filename == '\0')
    {
        error_print("the filename is empty", 0);
        return 0;
    }
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (!fd)
    {
        error_print("error creating the file", 0);
        return 0;
    }
    return (1);
    /* Creates a new file if it doesn't exist
    If file exists, it DELETES ALL EXISTING CONTENt Always starts with a clean /
        empty file Destructive operation(removes previous content)*/
}
static int validate_delimiter(const char *delimiter)
{
    if (!delimiter || *delimiter == '\0')
    {
        return 0;
    }
    return 1;
}

static int contain_char(char *str, char c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

int ft_index(char *str, char c)
{
	int i = 0;

	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return 0;
}

char *cut_from_op(char op, char *str, t_env *env)
{
    int pos;
    int count = 0;
    int start;
    int end;
    char *value;
    char *send;
    int i = 0;

    int len = ft_strlen(str);
    pos = ft_index(str, op);  
    if (pos < 0)  
        return NULL;
    start = pos + 1;
    while(str[start] && str[start] != ' ' && str[start] != '\t')
    {
        count++;
        start++;
    }
    end = start;
    
    value = (char *)malloc(sizeof(char) * (count + 1));
    if (!value)
        return NULL;
        
    start = pos + 1;  // Reset start to just after $
    i = 0;  // Reset i
    while (str[start] && start < end)
        value[i++] = str[start++];
    value[i] = '\0';  // Add null terminator
	
    send = my_getenv(value, transform(env));
    return send;
}

static char *replace_variable(char *line, char *var_name, char *new_val) {
    char *pos = strstr(line, var_name);
    if (!pos)
        return ft_strdup(line);  
    size_t before_len = pos - line;
    size_t var_name_len = ft_strlen(var_name);
    size_t new_val_len = ft_strlen(new_val);
    size_t after_len = ft_strlen(pos + var_name_len);
    size_t new_len = before_len + new_val_len + after_len;
    char *new_line = malloc(new_len + 1);
    if (!new_line)
        return NULL;
	strncpy(new_line, line, before_len);                   
    ft_strcpy(new_line + before_len, new_val);                 
    ft_strcpy(new_line + before_len + new_val_len, pos + var_name_len); 
    
    return new_line;
}


static char *extract_variable(const char *line) {

    const char prefix = '$';
    char *var_start = strchr(line, prefix);
    if (!var_start) {
        return NULL;
    }
    char *var_end = var_start + 1; 
    while (*var_end && (isalnum(*var_end) || *var_end == '_')) {
        var_end++;
    }
    size_t var_len = var_end - var_start;
    char *variable = malloc(var_len + 1);
    if (!variable) {
        return NULL;
    }
    strncpy(variable, var_start, var_len);
    variable[var_len] = '\0';  
    
    return variable;
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
    {
        perror("pipe creation failed");
        return -1;
    }
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

int contain_list(char *str, t_token *tk)
{
    t_token *curr;

    curr = tk;
    while (curr)
    {
        if (strcmp(curr->cmd, str) == 0)
            return (1);
        curr = curr->next;
    }
    return (0);
}

int handle_redirection(t_token *tk, char **ft_env, t_env *env)
{
    t_token *curr;

    curr = tk;
    while (curr)
    {
        if (curr->type == T_DGREAT || curr->type == T_GREAT)
        {
            if (curr->next && curr->next->type == T_IDENTIFIER)
            {
                if (curr->type == T_DGREAT)
                    handle_dgreat(curr->next->cmd);
                else
                    handle_great(curr->next->cmd);
            }
        }
        else if (curr->type == T_DLESS || curr->type == T_LESS)
        {
            if (curr->next && curr->next->type == T_IDENTIFIER)
            {
                if (contain_list("<<", tk) == 1)
                {
                    handle_cat_heredoc(ft_env, env, tk);
                }
                // else
                //     handle_less(curr->next->cmd);
            }
        }
        curr = curr->next;
    }
    return (1);
}
