/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:41 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/05 18:28:11 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_tok(void *tok)
{
	t_cmdtoken *tofree;

	tofree = tok;
	free(tofree->str);
}

char	*get_command_str(void)
{
	char	*tmp;
	char	*res;

	tmp = readline(PROMPT);
	res = ft_strtrim(tmp, " \v\t\n\r");
	free(tmp);
	if (res && *res)
	{
		add_history(res);
		return (res);
	}
	if (res)
		free(res);
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) envp;
	(void) argv;
	char		*line;
	t_list		*cmd_tokens;
	t_cmdtree	*cmd_tree;

	rl_initialize();
	//init();
	while (1)
	{
		line = get_command_str();
		if (line)
		{
			printf("Read: %s\n\n", line);
			cmd_tokens = lexer(line);
			ft_parse_tree(&cmd_tree, cmd_tokens);
			print_tokens(cmd_tokens);
			expand(cmd_tokens->content, envp);
			free(line);
			//free de los tokens!!!
			ft_lstclear(&cmd_tokens, free_cmd_tok);
		}
	}
	return (0);
}
