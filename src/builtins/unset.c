/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:53:48 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 10:57:19 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_cmd_node *node, t_mshell_sack *sack)
{
	int	i;

	i = 0;
	node->is_builtin = 1;
	while (node->args[++i])
		ft_remove_env(sack, node->args[i]);
	return (0);
}
