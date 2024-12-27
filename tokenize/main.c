/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:55:58 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/27 21:52:03 by njoudieh42       ###   ########.fr       */
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

int	main(void)
{
	char	*input;
	t_token	*t_list;

	input = "echo \"he'\\\"llo\\\" 'world' | grep '\" 'hard\\|case' >>file.txt && echo 'done |\"nested' >< \"new|file\" && cat <<EOF | awk '{print $2}' | tr -d '\n' EOF";
	t_list = ft_tokenize(input);
	display_list(t_list);
	ft_free_token_list(&t_list);
	return (0);
}
