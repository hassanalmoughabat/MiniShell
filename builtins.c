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