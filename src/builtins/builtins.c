/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:58:52 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/22 15:27:40 by sacorder         ###   ########.fr       */
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
			ft_putendl_fd("MiniShell: exit: numeric arg required", STDERR_FILENO);
			ft_printexit(2, sack, print);
		}
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("MiniShell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		ft_printexit(ft_atoi(node->args[1]), sack, print);
		
	}
	ft_printexit(0, sack, print);
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
		path = ft_get_from_env(sack->envp, "HOME", NULL);
		if (!*path)
			return (ft_putendl_fd("HOME is not set", STDERR_FILENO), 1);
	}
	cwd = ft_get_from_env(sack->envp, "PWD", NULL);
	if (chdir(path) == -1)
	{
		perror(path);
		return (1);
	}
	path = ft_strjoin("OLDPWD=", cwd);
	ft_add_to_env(sack, path);
	free(path);
	cwd = get_cwd_str();
	if (!cwd)
		return (1);
	path = ft_strjoin("PWD=", cwd);
	ft_add_to_env(sack, path);
	free(path);
	free(cwd);
	return (0);
}

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
