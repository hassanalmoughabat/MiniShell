#include "includes/minishell.h"

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	error_print(char *str, int error_nb)
{
	ft_printf("error is %s; exit code: %d", str, error_nb);
}

void	error_print_free(char *str, int error_nb, char **cmd)
{
	ft_printf("error is %s; exit code: %d", str, error_nb);
	ft_free_tab(cmd);
}