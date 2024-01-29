/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:15:01 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/29 11:53:59 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

/*
	Waits for all remaining child processes,
	then returns the exit code of the provided last_pid
*/
static	int	ft_wait_all(int last_pid, t_cmd_node *last)
{
	int	status;
	int	last_exited;
	int	exit_code;

	last_exited = 0;
	exit_code = INT_MIN;
	while (last_exited != -1)
	{
		last_exited = waitpid(-1, &status, 0);
		if (last_exited == last_pid && last_pid != 0)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = WTERMSIG(status) + 128;
		}
	}
	if (exit_code == INT_MIN)
		return (last->exit_code);
	return (exit_code);
}

/*
	Executes the cmd_list inside the node,
	then waits for the completion of those processes.
	Also creates a backup of STDIN/OUT before pipes that
	is restored post execution.
*/
int	ft_exec_and_wait(t_cmdtree *tree_node, t_mshell_sack *sack)
{
	int			std_backup[2];
	int			last_pid;
	int			tmp;
	t_cmd_node	*last;

	ft_set_signal_print(1);
	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	g_is_exec = 1;
	last = ft_execute_lst(tree_node, sack, &last_pid);
	tmp = ft_wait_all(last_pid, last);
	ft_dup2(std_backup[0], STDIN_FILENO);
	ft_dup2(std_backup[1], STDOUT_FILENO);
	ft_close(std_backup[0]);
	ft_close(std_backup[1]);
	if (g_is_exec == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	g_is_exec = 0;
	ft_set_signal_print(0);
	ft_putstr_fd("\x1b[0m", STDOUT_FILENO);
	if (tree_node->cmd_list->is_builtin == 1 && !tree_node->cmd_list->next)
		return (tree_node->cmd_list->exit_code);
	return (tmp);
}

/*
	Iterates over the provided string with starting po = i
	As i is an int *, after calling this function the provided
	int position is after the brackets.
*/
void	ft_brackets(char *str, int *i)
{
	int	ctr;

	ctr = 1;
	while (str[++(*i)])
	{
		if (str[*i] == '\'')
			state_quote_delimiter(str, i, '\'');
		else if (str[*i] == '(')
			++ctr;
		else if (str[*i] == ')')
			--ctr;
		if (ctr == 0)
			break ;
	}
}

/*
	Given a logically expandable char *str,
	returns the corresponding left expanded node
*/
char	*ft_get_left_token(char *str)
{
	int		i;
	int		last;
	char	*tmp;
	char	*res;

	i = 0;
	last = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			state_quote_delimiter(str, &i, str[i]);
			if (str[i] == '\'' || str[i] == '\"')
				++i;
		}
		else if (str[i] == '(')
			ft_brackets(str, &i);
		else
			++i;
		if ((str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '|' && str[i + 1] == '|') || str[i] == ';')
			last = i;
	}
	tmp = ft_substr(str, 0, last);
	return (res = ft_strtrim(tmp, " \n\t\v\r"), free(tmp), res);
}

/*
	Given a logically expandable char *str,
	returns the corresponding right expanded node
*/
char	*ft_get_right_token(char *str)
{
	int		i;
	int		last;
	char	*tmp;
	char	*res;

	i = 0;
	last = 0;
	while (str[i])
		is_quote(str, &i, &last);
	if ((str[last] == '&' && str[last + 1] == '&')
		|| (str[last] == '|' && str[last + 1] == '|'))
		++last;
	tmp = ft_substr(str, last + 1, SIZE_T_MAX);
	return (res = ft_strtrim(tmp, " \n\t\v\r"), free(tmp), res);
}

/*

int	execute(t_cmdtree *tree_node, t_mshell_sack *sack)
{
	t_cmd_node	*last;
	char		*keyval;
	char		*nbrstr;

	last = NULL;
	tree_node->exit_code = 0;
	if (tree_node->left)
		tree_node->exit_code = execute(tree_node->left, sack);
	if (tree_node->right)
		if ((tree_node->exit_code == 0 && tree_node->is_logic == AND_MASK)
			|| (tree_node->exit_code != 0 && tree_node->is_logic == OR_MASK)
			|| (tree_node->is_logic == WAIT_MASK))
			tree_node->exit_code = execute(tree_node->right, sack);
	if (!tree_node->is_logic)
		tree_node->exit_code = ft_exec_and_wait(tree_node, sack, &last);
	if (last && tree_node->exit_code == -1)
		tree_node->exit_code = last->exit_code;
	nbrstr = ft_itoa(tree_node->exit_code);
	keyval = ft_strjoin("?=", nbrstr);
	ft_add_to_env(sack, keyval);
	return (free(keyval), free(nbrstr), tree_node->exit_code);
}
*/
