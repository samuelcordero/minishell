/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:58:46 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 23:00:46 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_cmd_node *node, t_mshell_sack *sack)
{
	node->is_builtin = 1;
	if (node->args[0] && node->args[1])
	{
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("MiniShell: export: too many arguments",
				STDERR_FILENO);
			return (1);
		}
		return (ft_add_to_env(sack, node->args[1]));
	}
	return (1);
}

int	ft_unset(t_cmd_node *node, t_mshell_sack *sack)
{
	node->is_builtin = 1;
	if (node->args[0] && node->args[1])
	{
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("MiniShell: unset: too many arguments",
				STDERR_FILENO);
			return (1);
		}
		return (ft_remove_env(sack, node->args[1]));
	}
	else
		ft_putendl_fd("MiniShell: unset: missing argument", STDERR_FILENO);
	return (1);
}
