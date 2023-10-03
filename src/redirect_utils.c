/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:06:20 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/02 16:37:10 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(char *command, char **envp)
{
	char	*path;
	char	**split_cmd;

	split_cmd = ft_split(command, ' ');
	path = extract_exec_path(envp, split_cmd[0]);
	if (!path)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		if (split_cmd[0])
			ft_putendl_fd(split_cmd[0], 2);
		ft_free_array(split_cmd);
		exit(127);
	}
	if (execve(path, split_cmd, envp) == -1)
		ft_perror_exit("execve", -1);
}

void	ft_mid_redirect(char *cmd, char **envp)
{
	int	pipe_array[2];
	int	pid;

	if (pipe(pipe_array) == -1)
		ft_perror_exit("pipe", 71);
	pid = fork();
	if (pid == -1)
		ft_perror_exit("fork", 71);
	if (pid)
	{
		ft_close(pipe_array[1]);
		ft_dup2(pipe_array[0], STDIN_FILENO);
		ft_close(pipe_array[0]);
	}
	else
	{
		ft_close(pipe_array[0]);
		ft_dup2(pipe_array[1], STDOUT_FILENO);
		ft_close(pipe_array[1]);
		execute_cmd(cmd, envp);
	}
}

int	ft_redirect_in(char *cmd, char **envp, char *if_path, int chained)
{
	int	pipe_array[2];
	int	pid;
	int	fd;

	fd = ft_open(if_path, O_RDONLY, 0644);
	if (fd == -1)
		return (fd);
	if (pipe(pipe_array) == -1)
		ft_perror_exit("pipe", 71);
	pid = fork();
	if (pid == -1)
		ft_perror_exit("fork", 71);
	if (pid)
	{
		ft_close(pipe_array[1]);
		ft_dup2(pipe_array[0], fd);
		ft_close(pipe_array[0]);
	}
	else
	{
		ft_close(pipe_array[0]);
		if (chained)
			ft_dup2(pipe_array[1], STDOUT_FILENO);
		ft_close(pipe_array[1]);
		execute_cmd(cmd, envp);
	}
	return (pid);
}

int	ft_redirect_out(char *cmd, char **envp, char *of_path, int flags, int chained)
{
	int	pipe_array[2];
	int	pid;
	int	fd;

	fd = ft_open(of_path, flags, 0644);
	if (fd == -1)
		return (fd);
	if (pipe(pipe_array) == -1)
		ft_perror_exit("pipe", 71);
	pid = fork();
	if (pid == -1)
		ft_perror_exit("fork", 71);
	if (pid)
	{
		ft_close(pipe_array[1]);
		if (chained)
			ft_dup2(pipe_array[0], STDIN_FILENO);
		ft_close(pipe_array[0]);
	}
	else
	{
		ft_close(pipe_array[0]);
		ft_dup2(fd, STDOUT_FILENO);
		ft_close(pipe_array[1]);
		execute_cmd(cmd, envp);
	}
	return (pid);
}
