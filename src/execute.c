/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:15:01 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/21 19:34:44 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	/*
		STEPS:
			1. Try opening files, redirect loops
			2. Redirect loop for pipes
			3. Find executable, some builtins may be runned in parent process
			4. fork if necesary, then execute 
	*/
static int	ft_exec_cmd(t_cmd_node *node, char **envp, int *pid)
{
	int		fd;
	int		pipe_fds[2];
	t_list	*files;
	char	*path;

	files = node->redirs_lst;
	while (files)
	{
		fd = ft_open((t_redir_tok *)(files->content));
		if (fd < 0)
			return (1);
		if (((t_redir_tok *)(files->content))->redir_type == INFILE_MASK
			|| ((t_redir_tok *)(files->content))->redir_type == HEREDOC_MASK)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		ft_close(fd);
		files = files->next;
	}
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	path = extract_exec_path(envp, node->args[0]);
	if (!path)
	{
		ft_putstr_fd("dash: command not found: ", 2);
		if (node->args[0])
			ft_putendl_fd(node->args[0], 2);
		return (127);
	}
	//if it is builtin execute in father
	*pid = fork();
	if (*pid < 0)
		return (perror("fork"), 1);
	if (*pid)
	{
		ft_close(pipe_fds[1]);
		if (node->pipe_in)
			ft_dup2(STDIN_FILENO, pipe_fds[0]);
		ft_close(pipe_fds[0]);
	}
	else
	{
		ft_close(pipe_fds[0]);
		if (node->pipe_out)
			ft_dup2(STDOUT_FILENO, pipe_fds[1]);
		ft_close(pipe_fds[1]);
		if (execve(path, node->args, envp) == -1)
			return (perror("execve"), 1);
	}
	return (0);
}

static int	ft_execute_lst(t_cmdtree *t_node, char **envp, int *last_pid)
{
	t_cmd_node	*lst;
	int			tmp;

	lst = t_node->cmd_list;
	tmp = 0;
	while (lst)
	{
		if (ft_exec_cmd(lst, envp, &tmp) == 0)
			*last_pid = tmp;
		lst = lst->next;
	}
	return (0);
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
	int	last_pid;

	last_pid = 0;
	if (t_node->left)
		execute(t_node->left, envp);
	if (t_node->right)
		execute(t_node->right, envp);
	ft_execute_lst(t_node, envp, &last_pid);
	return (ft_wait_all(last_pid));
}
