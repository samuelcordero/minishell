/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:15:01 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/20 14:30:45 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_exec_cmd(t_cmd_node *node)
{
	/*
		STEPS:
			1. Try opening files, redirect loops
			2. Redirect loop for pipes
			3. Find executable, some builtins may be runned in parent process
			4. fork if necesary, then execute 
	*/
	(void)node;
	return (0);
}

static int	ft_execute_lst(t_cmdtree *t_node)
{
	t_cmd_node	*lst;

	lst = t_node->cmd_list;
	while (lst)
	{
		ft_exec_cmd(lst);
		lst = lst->next;
	}
	return (0);
}

int	execute(t_cmdtree *t_node)
{
	if (t_node->left)
		execute(t_node->left);
	if (t_node->right)
		execute(t_node->right);
	ft_execute_lst(t_node);
	//wait-list
	return (0);
}
