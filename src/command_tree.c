/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/17 13:56:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_fill_cmdlist(t_list *begin, t_list *end, t_cmdtree *tree_node)
{
	t_cmdtoken	*tkn;
	t_cmd_node	*current;
	t_cmd_node	*p_curr;
	int			i;

	current = ft_calloc(1, sizeof(t_cmd_node)); //memory error handling!
	p_curr = current;
	tkn = begin->content;
	tree_node->cmd_list = current;
	current->args = ft_calloc(100, sizeof(char *)); //maybe count args???? 100 is just eyeballing + error handling
	if (tkn->type == 2) //pipe in check
	{
		current->pipe_in = 1;
		begin = begin->next;
	}
	i = 0;
	while (begin != end)
	{
		tkn = begin->content;
		if (tkn->type == 0)
			current->args[i++] = tkn->str; // having problems freeing? strdup'it!
		else if (tkn->type == 1) // file in check
		{
			current->file_redirect += 1; //extract it from tkn->str
			if (!begin->next)
				//bad syntax
			begin = begin->next;
			tkn = begin->content;
			//current->if_name; || current->of_name = tkn->str;
		}
		else if (tkn->type == 2) // pipeout check, allocation for next list item
		{
			current->pipe_out = 1;
			current = ft_calloc(1, sizeof(t_cmd_node)); //error handling
			p_curr->next = current;
			p_curr = current;
			i = 0;
			current->args = ft_calloc(100, sizeof(char *)); //same shit sherlock, eyeballing + error handling
		}
		begin = begin->next;
	}
	return (0);
}

int	ft_parse_tree(t_cmdtree **tree, t_list *tokenlist)
{
	t_cmdtoken	*tkn;
	t_list		*needle;

	if (!tokenlist)
		return (1);
	*tree = NULL;
	*tree = ft_calloc(1, sizeof(t_cmdtree));
	if (!(*tree))
		return (1);
	while (tokenlist)
	{
		needle = tokenlist;
		while (needle)
		{
			tkn = needle->content;
			if (tkn->type > 1)
				break ;
			needle = needle->next;
		}
		if (ft_fill_cmdlist(tokenlist, needle, *tree))
			return (1);
		tokenlist = needle;
	}
	return (0);
}
