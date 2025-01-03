#include "includes/minishell.h"

void	display_list(t_token *list)
{
	t_token *current;

	current = list;
	while (current)
	{
		ft_printf("type: %d\n", current->type);
		ft_printf("type: %s\n", current->cmd);
		current = current->next;
	}
}