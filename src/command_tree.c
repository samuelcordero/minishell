/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/05 18:26:47 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int token_handler(t_cmdtoken *tkn, t_cmdtree **tree, t_cmd *tmp)
{
	
}

int	ft_parse_tree(t_cmdtree **tree, t_list *tokenlist)
{
	t_cmdtoken	*tkn;
	t_cmd		*tmp;

	if (!tokenlist)
		return (1);
	tmp = NULL;
	*tree = NULL;
	*tree = malloc(sizeof(t_cmdtree));
	if (!(*tree))
		return (1);
	while (tokenlist)
	{
		if (!tmp)
			tmp = malloc(sizeof(t_cmd));		
		tkn = tokenlist->content;
		token_handler(tkn, tree, tmp);
		tokenlist = tokenlist->next;
	}
	return (0);
}
