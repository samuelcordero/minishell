/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/10 14:26:52 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_set_exec_args(t_list *bg, t_list *end, t_cmd *curr)
{
	t_cmdtoken	*tkn;
	int			ctr;
	t_list		*tmp;

	ctr = 0;
	tmp = bg;
	while (bg)
	{
		tkn = bg->content;
		if (!tkn->type)
			ctr++;
		bg = bg->next;
	}
	if (ctr)
		curr->args = ft_calloc(ctr + 1, sizeof(char *));
	ctr = 0;
	while (tmp)
	{
		tkn = tmp->content;
		if (!tkn->type)
			curr->args[ctr++] = tkn->str;
		tmp = tmp->next;
	}
	return (0);
}

static int	ft_check_redirects(t_list *bg, t_list *end, t_cmd *curr)
{
	t_cmdtoken	*tkn;

	while (bg)
	{
		tkn = bg->content;
		if (tkn->type == 1)
			return (tkn->type);
		if (tkn->type == 2)
		{
			
			return (2);
		}
		bg = bg->next;
	}
	return (0);
}

static int	ft_cmd_constructor(t_list *begin, t_list *end, t_cmdtree **tree)
{
	t_list		*tmp;
	t_cmdtoken	*tkn;
	int			chained;
	t_cmd		*current;


	chained = 0;
	tmp = begin;
	tkn = begin->content;
	current = ft_calloc(1, sizeof(t_cmd));
	if (tkn->type == 2)
	{	
		tmp = tmp->next;
		current->pipe_in = 1;
	}
	else if (tkn->type == 3)
	{
		//handle tree priority with token
		(void) end;
		tmp = tmp->next;
	}
	current->next = NULL;
	current->redirect_type = ft_check_redirects(tmp, end);
	ft_set_exec_args(tmp, end, current);
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
		if (!(*tree)->cmd_list)
			(*tree)->cmd_list = ft_calloc(1, sizeof(t_cmd));
		if (!(*tree)->cmd_list)
			return (1);
		needle = tokenlist;
		while (needle)
		{
			tkn = needle->content;
			if (tkn->type > 1)
				break ;
			needle = needle->next;
		}
		if (ft_cmd_constructor(tokenlist, needle, tree))
			return (1);
		tokenlist = needle;
	}
	return (0);
}
