#include "includes/minihell.h"


int contain_more_than_command(t_token *tk, char **ft_env)
{
	t_token *curr;

	curr = tk;
	while (curr)
	{
		
	}
}
void	handle_pipe(t_token *lst, char **ft_env, t_env * env)
{

}

void handle_redirection(t_token *tk, char **ft_env, t_env *env)
{

}



void handle_exec(t_token *tk, char **ft_env, t_env *env)



int	is_pipe(char *value)
{
	int i = 0;

	while (value[i])
	{
		if (value[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

int is_redirection(char *value)
{
	int i = 0;

	while (value[i])
	{
		if (value[i] == '<' || value[i] == '>')
			return (1);
		else if((value[i] && value[i + 1]) && value[i] == '<' && value[i + 1] == '<')
			return (1);
		else if ((value[i] && value[i + 1]) && value[i] == '>' && value[i + 1] == '>')
			return (1);
		i++;
	}
	return (0);
}

int is_pipe()
{
	
}

void	after_parsing(t_token *tk, char **ft_env, t_env *env)
{
	ft_printf("in after parsing\n");
	t_token *curr;
	int command = 0;

	curr = tk;
	while (curr)
	{
		if (is_pipe(curr->cmd))
			handle_pipe(tk, ft_env, env);
		else if (is_redirection(curr->cmd))
			handle_redirection(tk, ft_env, env);
		else if (ft_is_builtin(curr->cmd))
			handle_builtin(tk, ft_env, env);
		else if (is_a_path_command(curr->cmd, ft_env) == 1)
			handle_path_command(tk, ft_env, env);
		curr = curr->next;
	}
}