/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:58:46 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/06 16:46:35 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isbuiltin(char *str)
{
	if (!ft_strncmp(str, "cd", 3))
		return (1);
	else if (!ft_strncmp(str, "echo", 5))
		return (1);
	else if (!ft_strncmp(str, "exit", 5))
		return (1);
	else if (!ft_strncmp(str, "pwd", 4))
		return (1);
	else if (!ft_strncmp(str, "unset", 6))
		return (1);
	else if (!ft_strncmp(str, "export", 7))
		return (1);
	return (0);
}

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
		if (!ft_strchr(node->args[1], '='))
			return (0);
		return (ft_add_to_env(sack, node->args[1]));
	}
	else
		ft_putendl_fd("MiniShell: export: missing argument", STDERR_FILENO);
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
