/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:41 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/19 21:24:31 by sacorder         ###   ########.fr       */
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

static void	print_cmdtree(t_cmdtree *head)
{
	t_cmd_node	*lst;
	int			i;

	lst = head->cmd_list;
	if ((int)head->is_logic == OR_MASK)
		printf("\n-----OR NODE-----\n");
	else if ((int)head->is_logic == AND_MASK)
		printf("\n-----AND NODE-----\n");
	else if ((int)head->is_logic == WAIT_MASK)
		printf("\n-----WAIT NODE-----\n");
	if (head->left)
	{
		printf("LEFT (MAIN) BRANCH:");
		print_cmdtree(head->left);
	}
	if (head->right)
	{
		printf("RIGHT (OPTIONAL) BRANCH:");
		print_cmdtree(head->right);
	}
	if (lst)
		printf("\n\nCommand list:\n\n");
	while (lst)
	{
		i = -1;
		while (lst->args[++i])
			printf("Arg %i: %s\n", i, lst->args[i]);
		printf("If, of: %s, %s\nPipe in/out: %i/%i\nFile redir: %i\n\n", lst->if_name, lst->of_name, (int)lst->pipe_in, (int)lst->pipe_out, (int)lst->file_redirect);
		lst = lst->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	t_mshell_sack	m_sack;
	t_list	*tmp;

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
			tmp = m_sack.cmd_tokens;
			if (ft_parse_tree(&m_sack.cmd_tree, &tmp))
				return (1);
			print_cmdtree(m_sack.cmd_tree);
			free(m_sack.line);
			//free de los tokens!!!
			ft_lstclear(&m_sack.cmd_tokens, free_cmd_tok);
			//free del arbol!!!
		}
	}
	return (0);
}
/* FOR LEAK TESTING

	atexit(leaks);

void	leaks(void)
{
	system("leaks minishell");
}
 FOR LEAK TESTING */

