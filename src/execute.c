/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:15:01 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/27 16:21:41 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

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

static int	ft_exec_and_wait(t_cmdtree *tree_node, t_mshell_sack *sack,
	t_cmd_node **last)
{
	int	std_backup[2];
	int	last_pid;
	int	tmp;

	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	g_is_exec = 1;
	*last = ft_execute_lst(tree_node, sack, &last_pid);
	tmp = ft_wait_all(last_pid);
	g_is_exec = 0;
	ft_dup2(std_backup[0], STDIN_FILENO);
	ft_dup2(std_backup[1], STDOUT_FILENO);
	ft_close(std_backup[0]);
	ft_close(std_backup[1]);
	ft_putstr_fd("\x1b[0m", STDIN_FILENO);
	return (tmp);
}

int	execute(t_cmdtree *tree_node, t_mshell_sack *sack)
{
	int			exit_code;
	t_cmd_node	*last;
	int			tmp;

	exit_code = 0;
	if (tree_node->left)
		exit_code = execute(tree_node->left, sack);
	if (tree_node->right)
		if ((exit_code == 0 && tree_node->is_logic == AND_MASK)
			|| (exit_code != 0 && tree_node->is_logic == OR_MASK)
			|| (tree_node->is_logic == WAIT_MASK))
			return (execute(tree_node->right, sack));
	tmp = ft_exec_and_wait(tree_node, sack, &last);
	if (last && last->exit_code != tmp)
		return (last->exit_code);
	return (tmp);
}
