/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:55:58 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/24 22:07:20 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/token.h"

void	display_list(t_token *list)
{
	t_token	*current;

	current = list;
	while (current)
	{
		printf("type: %d\n", current->type);
		printf("cmd: %s\n", current->cmd);
		current = current->next;
	}
}

t_token	ft_tokenize(char *input)
{
	t_token	*head_list;

	head_list = NULL;
	ft_check_string(input, head_list);
	display_list(head_list);
}

int	main(int argc, char ** argv,char **envp)
{
	((void)argv);
	((void)argc);
	
	char	*input;

		input = readline("minishell>");
		if (!input)
			exit(0);
		if(input[0])
			add_history(input);
		ft_tokenize(input);
	
}
