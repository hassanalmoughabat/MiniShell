/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njoudieh42 <njoudieh42>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:52:56 by njoudieh          #+#    #+#             */
/*   Updated: 2024/12/24 22:05:40 by njoudieh42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

//when i exit if!input i should clear the minishell
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
