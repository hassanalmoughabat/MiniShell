#include "includes/minishell.h"





static void init_shell(t_shell *shell, char **envp)
{
	shell->env = initialize_env_list(envp);
}

void	ft_read(char	*input)
{
	t_shell shell;

	input = NULL;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			ft_printf("\n");
			break;
		}
		if (input)
		{
			add_history(input);
			exec(input, (char **)shell.env);
		}
		free(input);
	}

}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char *input;

    (void)argc;
    (void)argv;

    init_shell(&shell, envp);
	ft_read(input);

   
}