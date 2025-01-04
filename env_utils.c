#include "includes/minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while(s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}	

void	free_my_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->line)
			free(tmp->line);
		if (tmp)
			free(tmp);
	}
}



t_env *create_env_node(char *envp)
{
	t_env *new_node;
	int i = 0;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	  new_node->line = strdup(envp);
        if (!new_node->line)
        {
            free(new_node);
            return (NULL);
        }
					
        new_node->next = NULL;
	return new_node;
}

t_env *initialize_env_list(char **envp)
{
    t_env *head;
    t_env *current;
    t_env *new_node;
    int i;

    head = NULL;
    i = 0;
    while (envp[i])
    {
        new_node = create_env_node(envp[i]);
        if (!new_node)
		{
			free_my_env(head);
            return (NULL);
		}   
        if (!head)
            head = new_node;
        else
            current->next = new_node;
        current = new_node;
        i++;
    }
    return (head);
}
