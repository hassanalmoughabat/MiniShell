#include "../includes/minihell.h"

static int	ft_find_pwd(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1] && str[i + 2])
	{
		if (str[i] == 'P' && str[i + 1] == 'W' && str[i + 2] == 'D')
		{
				ft_printf("\ncurent->line is %s\n", str);
				return (1);
		}
		i++;
	} 
	return (0);
}

static void	ft_print_pwd(char *str)
{
	int i = 3;

	while (str[++i])
		ft_printf("%c", str[i]);

}


int	ft_pwd(t_env *env)
{
	t_env *current;

	current = env;
	while (current)
	{
		if (ft_find_pwd(current->line) == 1)
		{	
			ft_print_pwd(current->line);
			return (1);
		}
		else 
			current = current->next;
	}

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

void	ft_cd(t_token *tk, t_env *env)
{

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
			ft_pwd(env);
		// else if (ft_strcmp(curr->cmd, "cd") == 0)
		// 	ft_cd(tk, env, ft_env);
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