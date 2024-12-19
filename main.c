/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh <njoudieh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:52:56 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/17 11:48:59 by njoudieh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//when i exit if!input i should clear the minishell
int	main(int argc, char ** argv,char **envp)
{
	((void)argv);
	((void)argc);
	
	char	*input;

	while (1)
	{
		input = readline("minishell>");
		if (!input)
			exit(0);
		if(input[0])
			add_history(input);
		ft_tokenize(input);
	}
	tree_formation(argc, argv);
}

/*tree_formation(char **argv){
ft_tokenize(argv)
}