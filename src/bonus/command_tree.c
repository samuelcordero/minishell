/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/12 14:46:02 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	red_compare(t_list *tkn_lst, t_list **tmp)
{
	if (ft_strncmp("<", ((t_cmdtoken *)tkn_lst->content)->str, 2) == 0)
		((t_redir_tok *)(*tmp)->content)->redir_type = INFILE_MASK;
	else if (ft_strncmp(">", ((t_cmdtoken *)tkn_lst->content)->str, 2) == 0)
		((t_redir_tok *)(*tmp)->content)->redir_type = OUTFILE_MASK;
	else if (ft_strncmp("<<", ((t_cmdtoken *)tkn_lst->content)->str, 3) == 0)
		((t_redir_tok *)(*tmp)->content)->redir_type = HEREDOC_MASK;
	else if (ft_strncmp(">>", ((t_cmdtoken *)tkn_lst->content)->str, 3) == 0)
		((t_redir_tok *)(*tmp)->content)->redir_type = CONCATOUT_MASK;
}

static int	set_file_info(t_list *tkn_lst, t_cmd_node *current)
{
	t_list		*tmp;

	if (!tkn_lst->next)
		return (1);
	tmp = current->redirs_lst;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (tmp)
	{
		tmp->next = ft_calloc(1, sizeof(t_list));
		tmp = tmp->next;
	}
	else
	{
		current->redirs_lst = ft_calloc(1, sizeof(t_list));
		tmp = current->redirs_lst;
	}
	tmp->content = ft_calloc(1, sizeof(t_redir_tok));
	((t_redir_tok *)tmp->content)->file_name = ft_strdup(((t_cmdtoken *)
				tkn_lst->next->content)->str);
	red_compare(tkn_lst, &tmp);
	return (0);
}

static int	ft_count_args(t_list *begin)
{
	t_cmdtoken	*tkn;
	int			res;

	res = 0;
	if (begin)
		tkn = begin->content;
	while (begin && tkn->type != PIPE)
	{
		if (tkn->type == ARG || tkn->type == EXP_ARG)
			++res;
		begin = begin->next;
		if (begin)
			tkn = begin->content;
	}
	return (res);
}

static int	fill_management(t_list **begin, t_cmd_node **current,
		t_cmd_node **p_curr, int *i)
{
	if (((t_cmdtoken *)(*begin)->content)->type == ARG || ((t_cmdtoken *)(*begin)->content)->type == EXP_ARG)
		(*current)->args[(*i)++] = ft_strdup(((t_cmdtoken *)
					(*begin)->content)->str);
	else if (((t_cmdtoken *)(*begin)->content)->type == FILE_REDIR)
	{
		if (set_file_info((*begin), (*current)))
			return (1);
		(*begin) = (*begin)->next;
	}
	else if (((t_cmdtoken *)(*begin)->content)->type == PIPE)
	{
		(*current) = ft_calloc(1, sizeof(t_cmd_node));
		if (!(*current))
			return (2);
		(*p_curr)->next = (*current);
		(*p_curr) = (*current);
		(*current)->args = ft_calloc(ft_count_args((*begin)->next) + 1,
				sizeof(char *));
		(*i) = 0;
		if (!(*current)->args)
			return (2);
	}
	return (0);
}
/*
	Given a t_list *begin that represents the begining of a token list,
	creates a cmd_list parsed for executor inside the given t_cmdtree *tree_node
*/

int	ft_fill_cmdlist(t_list *begin, t_cmdtree *tree_node)
{
	t_cmdtoken	*tkn;
	t_cmd_node	*current;
	t_cmd_node	*p_curr;
	int			ctr[2];

	current = ft_calloc(1, sizeof(t_cmd_node));
	if (!current)
		return (2);
	p_curr = current;
	tkn = begin->content;
	tree_node->cmd_list = current;
	current->args = ft_calloc(ft_count_args(begin) + 1, sizeof(char *));
	if (!current->args)
		return (2);
	if (tkn->type == PIPE)
		begin = begin->next;
	ctr[0] = 0;
	while (begin)
	{
		ctr[1] = fill_management(&begin, &current, &p_curr, &ctr[0]);
		if (ctr[1])
			return (ctr[1]);
		begin = begin->next;
	}
	return (0);
}

/* static	int	ft_rearrange(t_cmdtree **tree, t_list **needle)
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
} */
