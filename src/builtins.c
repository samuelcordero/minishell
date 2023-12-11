/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:58:52 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/06 17:02:24 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_msh_exit(t_cmd_node *node, t_mshell_sack *sack)
{
	if (node->args[0] && node->args[1])
	{
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("MiniShell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		ft_printexit(ft_atoi(node->args[1]), sack);
	}
	ft_printexit(0, sack);
	return (0);
}

int	ft_change_dir(t_cmd_node *node, t_mshell_sack *sack)
{
	char	*path;
	char	*cwd;

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
		path = ft_get_from_env(sack->envp, "HOME");
		if (!*path)
			return (ft_putendl_fd("HOME is not set", STDERR_FILENO), 1);
	}
	cwd = get_cwd_str();
	if (!cwd)
		return (1);
	if (chdir(path) == -1)
	{
		perror(path);
		return (1);
	}
	path = ft_strjoin("OLDPWD=", cwd);
	ft_add_to_env(sack, path);
	free(path);
	free(cwd);
	cwd = get_cwd_str();
	if (!cwd)
		return (1);
	path = ft_strjoin("PWD=", cwd);
	ft_add_to_env(sack, path);
	free(path);
	free(cwd);
	return (0);
}

int	ft_print_working_dir(t_cmd_node *node)
{
	char	*pwd;

	pwd = get_cwd_str();
	if (node->args[1])
		ft_putendl_fd("Minishell: pwd: too many args", STDERR_FILENO);
	else if (*pwd)
		ft_putendl_fd(pwd, STDOUT_FILENO);
	else
		ft_putendl_fd("PWD error", STDERR_FILENO);
	free(pwd);
	return (0);
}

int	ft_echo(t_cmd_node *node)
{
	char	flag;
	int		i;

	flag = 0;
	if (node->args[1] && !ft_strncmp(node->args[1], "-n", 3))
		flag = 1;
	i = flag;
	if (!node->pid)
	{
		while (node->args[++i])
		{
			ft_putstr_fd(node->args[i], STDOUT_FILENO);
			if (node->args[i + 1])
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
		if (!flag)
			ft_putendl_fd("", STDOUT_FILENO);
	}
	return (0);
}
