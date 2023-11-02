/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:19:31 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/02 13:19:22 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_shell_redir_fork(t_cmd_node *node)
{
	node->pid = fork();
	if (node->pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (node->pid)
	{
		ft_close(node->pipe_fds[1]);
		ft_dup2(node->pipe_fds[0], STDIN_FILENO);
		ft_close(node->pipe_fds[0]);
	}
}

static int	ft_no_path(t_cmd_node *node)
{
	ft_putstr_fd("MiniShell: command not found: ", 2);
	if (node->args[0])
		ft_putendl_fd(node->args[0], 2);
	node->exit_code = 127;
	return (0);
}

static int	ft_file_redirs(t_list *files)
{
	int		fd;

	fd = 0;
	while (files)
	{
		fd = ft_open((t_redir_tok *)(files->content));
		if (fd < 0)
			return (1);
		if (((t_redir_tok *)(files->content))->redir_type == INFILE_MASK)
			ft_dup2(fd, STDIN_FILENO);
		else if (((t_redir_tok *)(files->content))->redir_type == HEREDOC_MASK)
		{
			ft_dup2(fd, STDIN_FILENO);
			unlink(((t_redir_tok *)(files->content))->file_name);
		}
		else
			ft_dup2(fd, STDOUT_FILENO);
		ft_close(fd);
		files = files->next;
	}
	return (0);
}

	/*
		STEPS:
			1. Try opening files, redirect loops
			2. Redirect loop for pipes
			3. Find executable, some builtins may be runned in parent process
			4. fork if necesary, then execute 
	*/

static int	ft_exec_cmd(t_cmd_node *node, t_mshell_sack *sack)
{
	char	*path;

	if (ft_file_redirs(node->redirs_lst))
		return (1);
	if (pipe(node->pipe_fds) == -1)
		return (perror("pipe"), 1);
	path = extract_exec_path(sack, node);
	if (!path && node->is_builtin)
		return (0);
	if (!path)
		return (ft_no_path(node));
	ft_shell_redir_fork(node);
	if (!node->pid)
	{
		ft_close(node->pipe_fds[0]);
		if (node->pipe_out)
			ft_dup2(node->pipe_fds[1], STDOUT_FILENO);
		if (execve(path, node->args, sack->envp) == -1)
			return (perror(path), free(path), exit(126), 1);
		ft_close(node->pipe_fds[1]);
	}
	free(path);
	return (0);
}

t_cmd_node	*ft_execute_lst(t_cmdtree *tree_node,
	t_mshell_sack *sack, int *last_pid)
{
	t_cmd_node	*lst;

	lst = tree_node->cmd_list;
	while (lst)
	{
		if (ft_exec_cmd(lst, sack) == 0)
			*last_pid = lst->pid;
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}
