/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:53:50 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 18:39:01 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_management(t_mshell_sack *sack, char **path)
{
	char	*cwd;

	cwd = ft_get_from_env(sack->envp, "PWD", NULL);
	if (chdir(*path) == -1)
	{
		perror(*path);
		return (1);
	}
	*path = ft_strjoin("OLDPWD=", cwd);
	ft_add_to_env(sack, *path);
	free(*path);
	cwd = get_cwd_str();
	if (!cwd)
		return (1);
	*path = ft_strjoin("PWD=", cwd);
	ft_add_to_env(sack, *path);
	free(*path);
	free(cwd);
	return (0);
}

int	ft_change_dir(t_cmd_node *node, t_mshell_sack *sack)
{
	char	*path;

	if (node->args[0] && node->args[1])
	{
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("MiniShell: cd: too many arguments", STDERR_FILENO);
			return (1);
		}
		path = node->args[1];
	}
	else
	{
		path = ft_get_from_env(sack->envp, "HOME", NULL);
		if (!*path)
			return (ft_putendl_fd("HOME is not set", STDERR_FILENO), 1);
	}
	return (cd_management(sack, &path));
}
