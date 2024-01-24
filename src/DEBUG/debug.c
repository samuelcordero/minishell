/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:51:47 by guortun-          #+#    #+#             */
/*   Updated: 2024/01/24 12:12:30 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ternary(char is_builtin)
{
	debug_hub(__func__, NULL, NULL);
	if (is_builtin)
		debug_hub(NULL, "is_builtin", &is_builtin);
	else
		debug_hub(NULL, "is_builtin", &is_builtin);
	printf("\n-----------\n\n");
}

void	print_tokens(t_list *tokens)
{
	t_list		*current;
	t_cmdtkn	*ccontent;
	int			counter;

	current = tokens;
	counter = 0;
	while (current)
	{
		ccontent = (t_cmdtkn *)current->content;
		debug_hub(NULL, "counter", &counter);
		if (ccontent)
		{
			debug_hub(NULL, "ccontent->str", ccontent->str);
			debug_hub(NULL, "ccontent->type", &ccontent->type);
		}
		else
			debug_hub(NULL, "NULL\n", NULL);
		debug_hub(NULL, "--------------\n\n", NULL);
		counter++;
		current = current->next;
	}
}

static void	print_cmd_node(t_cmd_node *lst)
{
	t_list	*redirs_lst;
	int		i;

	if (lst)
		printf("\n\nCommand list:\n\n");
	while (lst)
	{
		i = -1;
		while (lst->args[++i])
			printf("Arg %i: %s\n", i, lst->args[i]);
		redirs_lst = lst->redirs_lst;
		while (redirs_lst)
		{
			printf("File redir type: %i, filename %s\n",
				(int)((t_redir_tok *)redirs_lst->content)->redir_type,
				((t_redir_tok *)redirs_lst->content)->file_name);
			redirs_lst = redirs_lst->next;
		}
		ternary(lst->is_builtin);
		lst = lst->next;
	}
}

void	print_cmdtree(t_cmdtree *head)
{
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
	print_cmd_node(head->cmd_list);
}

void	debug_hub(const char *functionName, const char *varname, void *var)
{
	if (DEBUG == 0)
		return ;
	else if (DEBUG == 1)
	{
		if (functionName == NULL)
			return ;
		printf("Checkpoint of %s\n", functionName);
	}
	else if (DEBUG == 2)
	{
		if (var == NULL)
			return ;
		print_variable(varname, var, DEBUG);
	}
}
