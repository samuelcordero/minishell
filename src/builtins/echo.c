/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:52:47 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 11:00:01 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd_node *node)
{
	char	flag;
	int		i;
	int		j;

	flag = 0;
	i = 1;
	if (node->args[1])
		while (node->args[i] && !ft_strncmp("-n", node->args[i], 2))
		{
			j = 0;
			while (node->args[i][++j])
				if (node->args[i][j] != 'n')
					break;
			if (node->args[i][j])
				break;
			flag = 1;
			++i;
		}
	if (!node->pid)
	{
		while (node->args[i])
		{
			ft_putstr_fd(node->args[i], STDOUT_FILENO);
			if (node->args[i + 1])
				ft_putstr_fd(" ", STDOUT_FILENO);
			++i;
		}
		if (!flag)
			ft_putendl_fd("", STDOUT_FILENO);
	}
	return (0);
}
