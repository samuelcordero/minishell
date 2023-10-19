/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/19 19:04:22 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_file_info(t_list *tkn_lst, t_cmd_node *current)
{
	if (!tkn_lst->next)
		return (1);
	if (ft_strncmp("<", ((t_cmdtoken *)tkn_lst->content)->str, 2) == 0)
	{
		current->file_redirect += INFILE_MASK;
		current->if_name = ((t_cmdtoken *)tkn_lst->next->content)->str;
	}
	else if (ft_strncmp(">", ((t_cmdtoken *)tkn_lst->content)->str, 2) == 0)
	{
		current->file_redirect += OUTFILE_MASK;
		current->of_name = ((t_cmdtoken *)tkn_lst->next->content)->str;
	}
	else if (ft_strncmp("<<", ((t_cmdtoken *)tkn_lst->content)->str, 3) == 0)
	{
		current->file_redirect += HEREDOC_MASK;
		current->if_name = ((t_cmdtoken *)tkn_lst->next->content)->str;
	}
	else if (ft_strncmp(">>", ((t_cmdtoken *)tkn_lst->content)->str, 3) == 0)
	{
		current->file_redirect += CONCATOUT_MASK;
		current->of_name = ((t_cmdtoken *)tkn_lst->next->content)->str;
	}
	return (0);
}

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
	if (tkn->type == PIPE) //pipe in check
	{
		current->pipe_in = 1;
		begin = begin->next;
	}
	i = 0;
	while (begin != end)
	{
		tkn = begin->content;
		if (tkn->type == ARG)
			current->args[i++] = tkn->str; // having problems freeing? strdup'it!
		else if (tkn->type == FILE_REDIR) // file io check
		{
			if (set_file_info(begin, current))
				return (1); //bad syntax, free and return error
			begin = begin->next;
		}
		else if (tkn->type == PIPE) // pipeout check, allocation for next list item
		{
			current->pipe_out = 1;
			current = ft_calloc(1, sizeof(t_cmd_node)); //error handling
			p_curr->next = current;
			p_curr = current;
			current->pipe_in = 1;
			i = 0;
			current->args = ft_calloc(100, sizeof(char *)); //same shit sherlock, eyeballing + error handling
		}
		begin = begin->next;
	}
	return (0);
}

static	int	ft_rearrange(t_cmdtree **tree, t_list **needle)
{
	t_cmdtree	*tmp;

	tmp = *tree;
	*tree = ft_calloc(1, sizeof(t_cmdtree));
	(*tree)->left = tmp;
	if (!(*needle)->next)
		return (1);
	if (!ft_strncmp(((t_cmdtoken *)((*needle)->content))->str, ";", 2))
		(*tree)->is_logic = WAIT_MASK;
	else if (!ft_strncmp(((t_cmdtoken *)((*needle)->content))->str, "&&", 3))
		(*tree)->is_logic = AND_MASK;
	else if (!ft_strncmp(((t_cmdtoken *)((*needle)->content))->str, "||", 3))
		(*tree)->is_logic = OR_MASK;
	(*needle) = (*needle)->next;
	return (ft_parse_tree(&((*tree)->right), needle));
}

int	ft_parse_tree(t_cmdtree **tree, t_list **tokenlist)
{
	t_cmdtoken	*tkn;
	t_list		*needle;

	if (!(*tokenlist))
		return (1);
	*tree = NULL;
	*tree = ft_calloc(1, sizeof(t_cmdtree));
	if (!(*tree))
		return (1);
	while ((*tokenlist))
	{
		needle = (*tokenlist);
		while (needle)
		{
			tkn = needle->content;
			if (tkn->type == 3)
				break ;
			needle = needle->next;
		}
		if (ft_fill_cmdlist((*tokenlist), needle, *tree))
			return (1);
		if (needle)
			if (ft_rearrange(tree, &needle))
				return (1);
		(*tokenlist) = needle;
	}
	return (0);
}
