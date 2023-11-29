/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:07:14 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/29 13:25:05 by sacorder         ###   ########.fr       */
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

/*
	Recursive freeing for cmd_tree and its contents
*/
void	*ft_free_cmdtree(t_cmdtree *tree)
{
	if (tree->left)
		ft_free_cmdtree(tree->left);
	if (tree->right)
		ft_free_cmdtree(tree->right);
	ft_free_cmd_list(tree->cmd_list);
	if (tree->cmd_str)
		free(tree->cmd_str);
	if (tree->expanded_str)
		free(tree->expanded_str);
	if (tree->cmd_tokens)
		ft_lstclear(&tree->cmd_tokens, free_cmd_tok);
	free(tree);
	return (NULL);
}
