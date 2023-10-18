/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:41 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/08 22:29:06 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_tok(void *tok)
{
	t_cmdtoken *tofree;

	tofree = tok;
	free(tofree->str);
	free(tofree);
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
	(void) argv;
	t_mshell_sack	m_sack;

	rl_initialize();
	init(&m_sack, envp);
	while (1)
	{
		m_sack.line = get_command_str();
		if (m_sack.line)
		{
			printf("Read: %s\n\n", m_sack.line);
			m_sack.cmd_tokens = lexer(m_sack.line);
			print_tokens(m_sack.cmd_tokens);
			//expand expand(cmd_tokens->content, envp); (test mock, finish!)
			ft_parse_tree(&m_sack.cmd_tree, m_sack.cmd_tokens);
			/*if (!ft_parse_tree(&m_sack.cmd_tree, m_sack.cmd_tokens))
				execute_tree(&m_sack.cmd_tree);*/
			free(m_sack.line);
			//free de los tokens!!!
			ft_lstclear(&m_sack.cmd_tokens, free_cmd_tok);
			//free del arbol!!!
		}
	}
	return (0);
}
