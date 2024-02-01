/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:54:10 by sacorder          #+#    #+#             */
/*   Updated: 2024/02/01 01:05:52 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_check_exit_arg(char *arg)
{
	int		i;
	int		sign_ctr;
	int		digits;

	i = -1;
	sign_ctr = 0;
	digits = 0;
	while (arg[++i] && sign_ctr <= 1)
	{
		if (!ft_isdigit(arg[i]) && !ft_isspace(arg[i])
			&& (arg[i] != '+' && arg[i] != '-'))
			return (0);
		if (arg[i] == '+' || arg[i] == '-')
			++sign_ctr;
		if (ft_isdigit(arg[i]))
			++digits;
	}
	if (sign_ctr <= 1 && digits >= 1 && digits <= 20)
		return (1);
	return (0);
}

int	ft_msh_exit(t_cmd_node *node, t_mshell_sack *sack, char print)
{
	if (node->args[0] && node->args[1])
	{
		if (!ft_check_exit_arg(node->args[1]))
		{
			ft_putendl_fd("µShell: exit: numeric arg required",
				STDERR_FILENO);
			ft_printexit(2, sack, print);
		}
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("µShell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		ft_printexit(ft_atoi(node->args[1]), sack, print);
	}
	ft_printexit(0, sack, print);
	return (0);
}
