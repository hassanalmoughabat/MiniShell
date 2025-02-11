#include "../includes/minihell.h"

int is_a_path_command(char *cmd, char **ft_env)
{
	char 	*path;
	char 	buffer[1024];
	int 	i;
	int		j;
	int		k;

	k = 0;
	i = 0;
	j = 0;
	path = my_getenv("PATH", ft_env);
	if (!path)
		return (0);
	while (path[i])
	{
		j = 0;
		while (path[i] && path[i] != ':')
			buffer[j++] = path[i++];
		buffer[j] = '/';
		j++;
		k = -1;
		while (cmd[++k])
			buffer[j++] = cmd[k];
		buffer[j] = '/0';
		if (access(buffer, X_OK) == 0) 
            return 1;
        if (path[i] == ':')
            i++;
    }
    return 0;
}


int contain_more_than_command(t_token *tk, char **ft_env)
{
	t_token *curr;

	curr = tk;
	while (curr)
	{
		
	}
}

// void	after_parsing(t_token *tk, char **ft_env)
// {

// }