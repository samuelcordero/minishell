/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:41 by sacorder          #+#    #+#             */
/*   Updated: 2023/07/05 17:18:46 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) envp;
	(void) argv;
	char	*line;
	char	*prompt="Mini\x1b[32mShell\x1b[0m > ";
	t_list	*cmd_tokens;
	
	while (1)
	{
		line = readline(prompt);
		printf("Read: %s\n\n", line);
		cmd_tokens = lexer(line);
		print_tokens(cmd_tokens);
		free(line);
		//free de los tokens!!!
	}
	return (0);
}