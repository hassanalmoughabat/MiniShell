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



void handle_builtin(t_token *tk, char **ft_env, t_env *env)
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
		// else if (ft_strcmp(curr->cmd, "exit") == 0)
		// 	ft_exit();
		// else if (ft_strcmp(curr->cmd, "export") == 0)
		// 	ft_export(tk, env, ft_env);
		// else if (ft_strcmp(curr->cmd, "unset") == 0)
		// 	ft_unset(tk, env, ft_env);
		// else if (ft_strcmp(curr->cmd, "echo") == 0)
		// 	ft_echo(tk, env, ft_env);
		curr = curr->next;
	}	
}