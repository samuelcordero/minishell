/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:15:01 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/23 17:00:27 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
			dup2(fd, STDIN_FILENO);
		else if (((t_redir_tok *)(files->content))->redir_type == HEREDOC_MASK)
		{
			dup2(fd, STDIN_FILENO);
			unlink(((t_redir_tok *)(files->content))->file_name);
		}
		else
			dup2(fd, STDOUT_FILENO);
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

static int	ft_exec_cmd(t_cmd_node *node, char **envp)
{
	char	*path;

	if (ft_file_redirs(node->redirs_lst))
		return (1);
	if (pipe(node->pipe_fds) == -1)
		return (perror("pipe"), 1);
	path = extract_exec_path(envp, node);
	if (!path && node->is_builtin)
		return (0);
	if (!path)
	{
		ft_putstr_fd("MiniShell: command not found: ", 2);
		if (node->args[0])
			ft_putendl_fd(node->args[0], 2);
		node->exit_code = 127;
		return (0);
	}
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
		if (execve(path, node->args, envp) == -1)
			return (perror("execve"), 1);
		ft_close(node->pipe_fds[1]);
	}
	return (0);
}

static t_cmd_node *ft_execute_lst(t_cmdtree *t_node, char **envp, int *last_pid)
{
	t_cmd_node	*lst;

	lst = t_node->cmd_list;
	while (lst)
	{
		if (ft_exec_cmd(lst, envp) == 0)
			*last_pid = lst->pid;
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

static	int	ft_wait_all(int last_pid)
{
	int	status;
	int	last_exited;
	int	exit_code;

	last_exited = 0;
	exit_code = 0;
	while (last_exited != -1)
	{
		last_exited = waitpid(-1, &status, 0);
		if (last_exited == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = WTERMSIG(status) + 128;
		}		
	}
	return (exit_code);
}

int	execute(t_cmdtree *t_node, char **envp)
{
	int			last_pid;
	int			exit_code;
	t_cmd_node	*last;
	int			tmp;
	int			std_backup[2];

	last_pid = 0;
	exit_code = 0;
	if (t_node->left)
		exit_code = execute(t_node->left, envp);
	if (t_node->right)
		if ((exit_code == 0 && t_node->is_logic == AND_MASK)
			|| (exit_code != 0 && t_node->is_logic == OR_MASK)
			|| (t_node->is_logic == WAIT_MASK))
			return (execute(t_node->right, envp));
	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	last = ft_execute_lst(t_node, envp, &last_pid);
	tmp = ft_wait_all(last_pid);
	dup2(std_backup[0], STDIN_FILENO);
	dup2(std_backup[1], STDOUT_FILENO);
	ft_close(std_backup[0]);
	ft_close(std_backup[1]);
	if (last && last->exit_code != tmp)
		return (last->exit_code);
	return  (tmp);
}
