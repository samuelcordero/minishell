/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:41 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/02 20:05:15 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_is_exec;

static char	*get_command_str(t_mshell_sack *sack)
{
	char	*tmp;
	char	*res;

	tmp = readline(sack->custom_prompt);
	sack->cmd_tokens = NULL;
	if (!tmp)
	{
		sack->eof = 1;
		return (NULL);
	}
	res = ft_strtrim(tmp, " \v\t\n\r");
	free(tmp);
	if (res && *res)
	{
		ft_add_history(res, sack);
		return (res);
	}
	if (res)
		free(res);
	return (NULL);
}

void	leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	t_mshell_sack	m_sack;
	t_list			*tmp;

	rl_initialize();
	init(&m_sack, envp, argc, argv);
	while (1)
	{
		m_sack.line = get_command_str(&m_sack);
		m_sack.expanded = NULL;
		if (m_sack.line && !m_sack.eof)
		{
			m_sack.expanded = ft_expand(m_sack.line, m_sack.envp);
			if (m_sack.expanded && *m_sack.expanded)
			{
				m_sack.cmd_tokens = lexer(m_sack.expanded);
				ft_remove_quotes(m_sack.cmd_tokens);
				tmp = m_sack.cmd_tokens;
				if (ft_parse_tree(&m_sack.cmd_tree, &tmp))
					return (ft_putendl_fd("Minishell: memory error", STDERR_FILENO), 1);
				execute(m_sack.cmd_tree, &m_sack);
				m_sack.last_exit = m_sack.cmd_tree->exit_code;
				ft_lstclear(&m_sack.cmd_tokens, free_cmd_tok);
				m_sack.cmd_tree = ft_free_cmdtree(m_sack.cmd_tree);
			}
			free(m_sack.expanded);
			free(m_sack.line);
		}
		else if (m_sack.eof)
			ft_printexit(0, &m_sack); //maybe clean exit
	}
	return (0);
}

/*FOR LEAK TESTING

	atexit(leaks);

void	leaks(void)
{
	system("leaks minishell");
}

PRINT CMD TREE
			//print_cmdtree(m_sack.cmd_tree);

void	print_cmdtree(t_cmdtree *head)
{
	t_cmd_node	*lst;
	t_list		*redirs_lst;
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
		printf("<LEFT (MAIN) BRANCH:>");
		print_cmdtree(head->left);
	}
	if (head->right)
	{
		printf("<RIGHT (OPTIONAL) BRANCH:>");
		print_cmdtree(head->right);
	}
	if (lst)
		printf("\n\nCommand list:\n\n");
	while (lst)
	{
		i = -1;
		while (lst->args[++i])
			printf("Arg %i: %s\n", i, lst->args[i]);
		printf("Pipe in/out: %i/%i\n", (int)lst->pipe_in, (int)lst->pipe_out);
		redirs_lst = lst->redirs_lst;
		while (redirs_lst)
		{
			printf("File redir type: %i, filename %s\n", (int)((t_redir_tok *)redirs_lst->content)->redir_type, ((t_redir_tok *)redirs_lst->content)->file_name);
			redirs_lst = redirs_lst->next;
		}
		printf("\n");
		lst = lst->next;
	}
}

			//print_tokens(m_sack.cmd_tokens);

 */
