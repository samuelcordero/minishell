/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:15:01 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/14 14:00:12 by sacorder         ###   ########.fr       */
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
	exit_code = -1;
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

static int	get_log_expandible(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '&' && str[i + 1] == '&')
			return (AND_MASK);
		else if (str[i] == '|' && str[i + 1] == '|')
			return (OR_MASK);
		else if (str[i] == ';')
			return (WAIT_MASK);
	}
	return (0);
}

static void ft_brackets(char *str, int *i)
{
	int ctr;

	ctr = 1;
	while (str[++(*i)])
	{
		if (str[*i] == '(')
		{
			++ctr;
			++(*i);
		}
		if (str[*i] == ')')
			--ctr;
		if (ctr == 0)
			break;
	}
}

static char	*ft_get_left_token(char *str)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			ft_brackets(str, &i);
		else
			++i;
		if ((str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '|' && str[i + 1] == '|')
			|| str[i] == ';')
			break;
	}
	tmp = ft_substr(str, 0, i);
	res = ft_strtrim(tmp, " \n\t\v\r");
	return (free(tmp), res);
}

static char	*ft_get_right_token(char *str)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			ft_brackets(str, &i);
		else
			++i;
		if ((str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '|' && str[i + 1] == '|')
			|| str[i] == ';')
			break;
	}
	if ((str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '|' && str[i + 1] == '|'))
		++i;
	tmp = ft_substr(str, i + 1, SIZE_T_MAX);
	res = ft_strtrim(tmp, " \n\t\v\r");
	return (free(tmp), res);
}

static void	logic_expansion(t_cmdtree *tree_node)
{
	char	*str;

	str = tree_node->cmd_str;
	tree_node->is_logic = get_log_expandible(str);
	if (tree_node->is_logic)
	{
		tree_node->left = ft_calloc(1, sizeof(t_cmdtree));
		tree_node->left->cmd_str = ft_get_left_token(str);
		//ft_putendl_fd(tree_node->left->cmd_str, STDERR_FILENO);
		tree_node->right = ft_calloc(1, sizeof(t_cmdtree));
		tree_node->right->cmd_str = ft_get_right_token(str);
		//ft_putendl_fd(tree_node->right->cmd_str, STDERR_FILENO);
	}
}

/*
	1. expand str
	2. tokenize
	3. create cmd lst
	4. exec and wait
*/
static int	ft_parse_and_exec(t_cmdtree *tree_node, t_mshell_sack *sack,
	t_cmd_node **last)
{
	tree_node->expanded_str = ft_expand(tree_node->cmd_str, sack->envp);
	tree_node->cmd_tokens = lexer(tree_node->expanded_str);
	ft_remove_quotes(tree_node->cmd_tokens);
	ft_fill_cmdlist(tree_node->cmd_tokens, NULL, tree_node);
	return (ft_exec_and_wait(tree_node, sack, last));
}

/*
	1. try to expand logically
	2. recursively call left, then right if left exited satisfying requirements
	3. base case, if not logic, try to expand env variables,
		tokenize and create cmd list, execute lst and wait
*/
int	expand_execute(t_cmdtree *tree_node, t_mshell_sack *sack)
{
	t_cmd_node	*last;
	char		*keyval;
	char		*nbrstr;

	last = NULL;
	logic_expansion(tree_node);
	if (tree_node->left)
		tree_node->exit_code = expand_execute(tree_node->left, sack);
	if (tree_node->right)
		if ((tree_node->exit_code == 0 && tree_node->is_logic == AND_MASK)
			|| (tree_node->exit_code != 0 && tree_node->is_logic == OR_MASK)
			|| (tree_node->is_logic == WAIT_MASK))
			tree_node->exit_code = expand_execute(tree_node->right, sack);
	if (!tree_node->is_logic)
		tree_node->exit_code = ft_parse_and_exec(tree_node, sack, &last);
	if (last && tree_node->exit_code == -1)
		tree_node->exit_code = last->exit_code;
	nbrstr = ft_itoa(tree_node->exit_code);
	keyval = ft_strjoin("?=", nbrstr);
	ft_add_to_env(sack, keyval);
	return (free(keyval), free(nbrstr), tree_node->exit_code);
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
