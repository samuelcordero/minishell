/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:58:52 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 22:59:01 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_msh_exit(t_cmd_node *node, char **envp)
{
	//maybe clean memory before exit
	(void)envp;
	node->is_builtin = 1;
	if (node->args[0] && node->args[1])
	{
		if (node->args[2] != NULL)
		{
			ft_putendl_fd("MiniShell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		ft_printexit(ft_atoi(node->args[1]));
	}
	ft_printexit(0);
	return (0);
}

int	ft_change_dir(t_cmd_node *node, t_mshell_sack *sack)
{
	char	*path;
	void	*nullable;
	char	cwd[256];

	node->is_builtin = 1;
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
		path = ft_get_from_env(sack->envp, "HOME");
	nullable = getcwd(cwd, sizeof(cwd));
	if (!nullable)
	{
		perror("MiniShell: update cwd error");
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror(path);
		return (1);
	}
	path = ft_strjoin("OLDPWD=", cwd);
	ft_add_to_env(sack, path);
	free(path);
	nullable = getcwd(cwd, sizeof(cwd));
	if (!nullable)
	{
		perror("MiniShell: update cwd error");
		return (1);
	}
	path = ft_strjoin("PWD=", cwd);
	ft_add_to_env(sack, path);
	free(path);
	return (0);
}

int	ft_print_working_dir(t_cmd_node *node, char **envp)
{
	void	*nullable;
	char	cwd[256];

	(void)envp;
	node->is_builtin = 1;
	node->pid = fork();
	if (node->pid < 0)
		return (perror("fork"), 1);
	if (node->pid)
	{
		ft_close(node->pipe_fds[1]);
		ft_dup2(node->pipe_fds[0], STDIN_FILENO);
		ft_close(node->pipe_fds[0]);
	}
	else
	{
		ft_close(node->pipe_fds[0]);
		if (node->pipe_out)
			ft_dup2(node->pipe_fds[1], STDOUT_FILENO);
		nullable = getcwd(cwd, sizeof(cwd));
		if (!nullable)
		{
			perror("MiniShell: pwd");
			return (1);
		}
		ft_putendl_fd(cwd, STDOUT_FILENO);
		ft_close(node->pipe_fds[1]);
		exit(0);
	}
	return (0);
}

int	ft_echo(t_cmd_node *node, char **envp)
{
	char	flag;
	int		i;

	(void)envp;
	node->is_builtin = 1;
	flag = 0;
	if (node->args[1] && !ft_strncmp(node->args[1], "-n", 3))
		flag = 1;
	i = flag;
	node->pid = fork();
	if (node->pid < 0)
		return (perror("fork"), 1);
	if (node->pid)
	{
		ft_close(node->pipe_fds[1]);
		ft_dup2(node->pipe_fds[0], STDIN_FILENO);
		ft_close(node->pipe_fds[0]);
	}
	else
	{
		ft_close(node->pipe_fds[0]);
		if (node->pipe_out)
			ft_dup2(node->pipe_fds[1], STDOUT_FILENO);
		while (node->args[++i])
		{
			ft_putstr_fd(node->args[i], STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		if (!flag)
			ft_putendl_fd("", STDOUT_FILENO);
		ft_close(node->pipe_fds[1]);
		exit(0);
	}
	return (0);
}
