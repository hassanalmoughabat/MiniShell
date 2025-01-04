#include "includes/minishell.h"


void	display_env(t_env	*head)
{
	t_env	*current = head;

	while (current != NULL)
	{
		ft_printf("LIine: %s\n", current->line);
		ft_printf("---------\n");
		current = current->next;
	}
}


static void init_shell(t_shell *shell, char **envp)
{
    shell->env = initialize_env_list(envp);
    shell->path = NULL;
    shell->tokens = NULL;
    shell->exit_status = 0;
    
    // Debug print to verify environment initialization
    ft_printf("DEBUG: Environment initialization ");
    if (shell->env)
        ft_printf("successful\n");
    else
        ft_printf("failed\n");
}

// void	ft_read(char	*input)
// {
// 	t_shell shell;

// 	input = NULL;
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (!input)
// 		{
// 			ft_printf("\n");
// 			break;
// 		}
// 		if (input)
// 		{
// 			add_history(input);
// 			ft_printf("\nbefore exec\n");
// 			exec(input, (char **)shell.env);
// 		}
// 		free(input);
// 	}

// }


void print_envp(char **envp) {
    int i = 0;

    while (envp[i] != NULL) {
        ft_printf("%s\n", envp[i]); // Print each environment variable
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char *input;
    (void)argc;
    (void)argv;

    ft_printf("Initializing shell...\n");
    init_shell(&shell, envp);

    // Debug: Print environment contents
    ft_printf("\nDEBUG: Environment contents:\n");
    t_env *current = shell.env;
    while (current)
    {
        ft_printf("%s\n", current->line);
        current = current->next;
    }

    ft_printf("\nGetting PATH...\n");
    char *path = get_my_path(shell.env);
    
    if (!path)
        ft_printf("Path is NULL\n");
    else
    {
        ft_printf("Path is: %s\n", path);
        shell.path = path;  // Store the path in shell structure
    }

    return (0);
}