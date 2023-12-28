/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 14:12:29 by guortun-          #+#    #+#             */
/*   Updated: 2023/12/28 14:26:33 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	exec_first_management(t_cmd_node *node, t_mshell_sack *sack, char **path)
{
	ft_dup2(node->pipe_fds[1], STDOUT_FILENO);
	ft_close(node->pipe_fds[0]);
	ft_close(node->pipe_fds[1]);
	if (ft_isbuiltin(node->args[0]))
		ft_execbuiltin(node, sack, 0);
	ft_envp_tidy(sack);
	if (execve(*path, node->args, sack->envp) == -1)
		return (perror(*path), free(*path), exit(126), 1);
}
static int	ft_exec_mid_cmd(t_cmd_node *node, t_mshell_sack *sack, int inputfd, int *outfd)
{
	char	*path;

	if (pipe(node->pipe_fds) == -1)
		return (perror("pipe"), 1);
	if (ft_file_redirs(node->redirs_lst, inputfd, node->pipe_fds[1], sack->envp))
		return (ft_close(inputfd), ft_close(node->pipe_fds[1]), *outfd = node->pipe_fds[0], node->exit_code = 1, 1);
	if (!node->args[0])
		return (ft_close(inputfd), ft_close(node->pipe_fds[1]), *outfd = node->pipe_fds[0], node->exit_code = 0, 1);
	path = extract_exec_path(sack, node);
	if (!path && !ft_isbuiltin(node->args[0]))
		return (ft_no_path(node, 1, node->pipe_fds[1]), ft_close(inputfd), *outfd = node->pipe_fds[0], 1);
	ft_fork(node);
	if (node->pid)
	{
		ft_close(node->pipe_fds[1]);
		ft_close(inputfd);
	}
	if (!node->pid)
	{
		ft_dup2(inputfd, STDIN_FILENO);
		return (exec_first_management(node, sack, &path));
	}
	free(path);
	*outfd = node->pipe_fds[0];
	return (0);
}
static int	ft_exec_last_cmd(t_cmd_node *node, t_mshell_sack *sack, int inputfd)
{
	char	*path;

	if (ft_file_redirs(node->redirs_lst, inputfd, STDOUT_FILENO, sack->envp))
		return (ft_close(inputfd), node->exit_code = 1, 1);
	if (!node->args[0])
		return (ft_close(inputfd), node->exit_code = 0, 1);
	path = extract_exec_path(sack, node);
	if (!path && !ft_isbuiltin(node->args[0]))
		return (ft_no_path(node, 1, STDOUT_FILENO), ft_close(inputfd), 0);
	ft_fork(node);
	if (node->pid)
		ft_close(inputfd);
	if (!node->pid)
	{
		ft_dup2(inputfd, STDIN_FILENO);
		if (ft_isbuiltin(node->args[0]))
			ft_execbuiltin(node, sack, 0);
		ft_envp_tidy(sack);
		if (execve(path, node->args, sack->envp) == -1)
			return (ft_close(inputfd),
				perror(path), free(path), exit(126), 1);
	}
	free(path);
	return (0);
}
t_cmd_node	*ft_execute_lst(t_cmdtree *tree_node,
	t_mshell_sack *sack, int *last_pid)
{
	t_cmd_node	*lst;
	int			inputfd = 0;
	int			outputfd = 0;

	lst = tree_node->cmd_list;
	if (!lst->next && lst->args[0] && ft_isbuiltin(lst->args[0]))
	{
		ft_execbuiltin(lst, sack, 1);
		lst->is_builtin = 1;
		return (lst);
	}
	else if (!lst->next)
	{
		ft_exec_single_cmd(lst, sack);
		return (*last_pid = lst->pid, lst);
	}
	ft_exec_first_cmd(lst, sack, &inputfd);
	lst = lst->next;
	while (lst->next)
	{
		ft_exec_mid_cmd(lst, sack, inputfd, &outputfd);
		inputfd = outputfd;
		lst = lst->next;
	}
	ft_exec_last_cmd(lst, sack, inputfd);
	*last_pid = lst->pid;
	return (lst);
}