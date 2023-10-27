/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:07:14 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/27 16:00:24 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_redirs(void *t)
{
	t_redir_tok	*tok;

	tok = t;
	free(tok->file_name);
	free(tok);
}

static void	ft_free_cmd_list(t_cmd_node *cmd_list)
{
	t_cmd_node	*next;

	while (cmd_list)
	{
		next = cmd_list->next;
		ft_lstclear(&cmd_list->redirs_lst, &clear_redirs);
		ft_free_array(cmd_list->args);
		free(cmd_list);
		cmd_list = next;
	}
}

void	ft_free_cmdtree(t_cmdtree *tree)
{
	if (tree->left)
		ft_free_cmdtree(tree->left);
	if (tree->right)
		ft_free_cmdtree(tree->right);
	ft_free_cmd_list(tree->cmd_list);
}
