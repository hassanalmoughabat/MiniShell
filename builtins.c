#include "../includes/minihell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];
	
	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return(1);
	}
	else
		return (0);
}

void	ft_print_env(t_env *env)
{
	t_env *ev;

	ev = env;
	while (ev)
	{
		ft_printf("%s\n", ev->line);
		ev = ev->next;
	}
}

void ft_exit(t_token *tk, char **ft_env, t_env *env, char *input)
{
    int exit_code = 0;
    t_token *curr = tk->next;
    
    ft_printf("exit\n");
    
    if (curr && curr->cmd)
        exit_code = ft_atoi(curr->cmd);
    
    // Free resources
    free_token_list(tk);
    free_env_list(env);
    free_array(ft_env);
    free(input);
    
    exit(exit_code % 256);
}

static void ft_echo(t_token *tk, t_env *env, char *input)
{
	t_token *curr;
	int flag = 0;
	curr = tk;
	while (curr)
	{
		if (ft_strcmp(curr->cmd, "echo") == 0)
		{
			curr = curr->next;
			continue;
		}

		if (ft_strcmp(curr->cmd, "-n") == 0)
		{
			flag = 1;
			curr = curr->next;
			continue;
		}

		if (curr->cmd && curr->cmd[0] == '$')
		{
			printf("here in $ check\n");
			char *env_value = my_getenv(curr->cmd + 1, transform(env));
			if (env_value && curr->cmd)
			{
				ft_printf("%s", env_value);
			}
			else
			{
				ft_printf("Variable not found\n");
			}
		}
		else if (curr->cmd)
		{
			ft_printf("%s", curr->cmd);
		}

		curr = curr->next;
	}
	if (!flag)
		ft_printf("\n");
}

void handle_builtin(t_token *tk, char **ft_env, t_env *env, char *input)
{
	t_token *curr;

	curr = tk;
	while (curr)
	{
		if (ft_strcmp(curr->cmd, "env") == 0)
			ft_print_env(env);
		else if (ft_strcmp(curr->cmd, "pwd") == 0)
			ft_pwd();
		else if (ft_strcmp(curr->cmd, "cd") == 0)
			ft_cd(tk, env, ft_env);
		else if (ft_strcmp(curr->cmd, "exit") == 0)
			ft_exit(tk, ft_env, env, input);
		else if (ft_strcmp(curr->cmd, "export") == 0)
			ft_export(input, env, ft_env);
		// else if (ft_strcmp(curr->cmd, "unset") == 0)
		// 	ft_unset(tk, env, ft_env);
		else if (ft_strcmp(curr->cmd, "echo") == 0)
			ft_echo(tk, env, input);
		curr = curr->next;
	}	
}