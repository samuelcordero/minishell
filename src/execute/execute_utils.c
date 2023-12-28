/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:19:31 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 14:28:39 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_fork(t_cmd_node *node)
{
	node->pid = fork();
	if (node->pid < 0)
	{
		perror("fork");
		exit(1);
	}
}

/*
	Prints command not found, then
	sets exit code as 127 and closes pipes
*/
static int	ft_no_path(t_cmd_node *node, int close, int fd)
{
	ft_putstr_fd("MiniShell: command not found: ", STDERR_FILENO);
	if (node->args[0])
		ft_putendl_fd(node->args[0], STDERR_FILENO);
	node->exit_code = 127;
	if (close)
		ft_close(fd);
	return (0);
}

int	ft_file_redirs(t_list *files, int input_fd, int output_fd, char **envp)
{
	int		fd;

	while (files)
	{
		fd = ft_open((t_redir_tok *)(files->content));
		if (fd < 0)
			return (1);
		if (((t_redir_tok *)(files->content))->redir_type == INFILE_MASK)
			ft_dup2(fd, input_fd);
		else if (((t_redir_tok *)(files->content))->redir_type == HEREDOC_MASK)
		{
			fd = ft_expand_heredoc(fd, (t_redir_tok *)(files->content), envp);
			ft_close(fd);
			fd = ft_open((t_redir_tok *)(files->content));
			if (fd < 0)
				return (1);
			ft_dup2(fd, input_fd);
			unlink(((t_redir_tok *)(files->content))->file_name);
		}
		else
			ft_dup2(fd, output_fd);
		ft_close(fd);
		files = files->next;
	}
	return (0);
}

/*
	(DEPRECATED ft_exec_cmd, rewrite)
	STEPS:
	1. Try opening files, redirect loops
	2. Redirect loop for pipes
	3. Find executable, some builtins may be runned in parent process
	4. fork if necesary, then execute 
*/
static int	ft_exec_single_cmd(t_cmd_node *node, t_mshell_sack *sack)
{
	char	*path;

	if (ft_file_redirs(node->redirs_lst, STDIN_FILENO,
			STDOUT_FILENO, sack->envp))
		return (node->exit_code = 1, 1);
	if (!node->args[0])
		return (1);
	path = extract_exec_path(sack, node);
	if (!path)
		return (ft_no_path(node, 0, 0), 0);
	ft_fork(node);
	if (node->pid)
		ft_close(node->pipe_fds[0]);
	if (!node->pid)
	{
		ft_envp_tidy(sack);
		if (execve(path, node->args, sack->envp) == -1)
			return (perror(path), free(path), exit(126), 1);
	}
	free(path);
	return (0);
}

int	ft_exec_first_cmd(t_cmd_node *node, t_mshell_sack *sack, int *outfd)
{
	char	*path;

	if (pipe(node->pipe_fds) == -1)
		return (perror("pipe"), 1);
	if (ft_file_redirs(node->redirs_lst,
			STDIN_FILENO, node->pipe_fds[1], sack->envp))
		return (ft_close(node->pipe_fds[1]), *outfd = node->pipe_fds[0],
			node->exit_code = 1, 1);
	if (!node->args[0])
		return (ft_close(node->pipe_fds[1]), *outfd = node->pipe_fds[0],
			node->exit_code = 0, 1);
	path = extract_exec_path(sack, node);
	if (!path && !ft_isbuiltin(node->args[0]))
		return (ft_no_path(node, 1, node->pipe_fds[1]),
			*outfd = node->pipe_fds[0], 0);
	ft_fork(node);
	if (node->pid)
		ft_close(node->pipe_fds[1]);
	if (!node->pid)
		return (exec_first_management(node, sack));
	free(path);
	*outfd = node->pipe_fds[0];
	return (0);
}
