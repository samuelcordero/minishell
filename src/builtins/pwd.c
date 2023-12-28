/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:53:52 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 10:56:25 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_working_dir(t_cmd_node *node, char **envp)
{
	char	*pwd;

	(void) node;
	pwd = ft_get_from_env(envp, "PWD", NULL);
	if (*pwd)
		ft_putendl_fd(pwd, STDOUT_FILENO);
	else
		ft_putendl_fd("PWD error", STDERR_FILENO);
	return (0);
}
