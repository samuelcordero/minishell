/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 16:20:15 by guortun-          #+#    #+#             */
/*   Updated: 2024/02/01 01:05:52 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_mask(char *str, int last)
{
	if (str[last] == '&' && str[last + 1] == '&')
		return (AND_MASK);
	else if (str[last] == '|' && str[last + 1] == '|')
		return (OR_MASK);
	else if (str[last] == ';')
		return (WAIT_MASK);
	return (0);
}

/*
	1. try to expand logically
	2. recursively call left, then right if left exited satisfying requirements
	3. base case, if not logic, try to expand env variables,
		tokenize and create cmd list, execute lst and wait
*/
int	expand_execute(t_cmdtree *tree_node, t_mshell_sack *sack)
{
	char		*keyval;
	char		*nbrstr;

	sack->cont = 1;
	if (logic_expansion(tree_node))
		return (ft_expansion_error(tree_node), ft_printexit(2, sack, 1), 2);
	if (tree_node->left)
		tree_node->exit_code = expand_execute(tree_node->left, sack);
	if (tree_node->right)
		if (((tree_node->exit_code == 0 && tree_node->is_logic == AND_MASK)
				|| (tree_node->exit_code != 0 && tree_node->is_logic == OR_MASK)
				|| (tree_node->is_logic == WAIT_MASK))
			&& sack->cont)
			tree_node->exit_code = expand_execute(tree_node->right, sack);
	if (!tree_node->is_logic)
		tree_node->exit_code = ft_parse_and_exec(tree_node, sack);
	nbrstr = ft_itoa(tree_node->exit_code);
	keyval = ft_strjoin("?=", nbrstr);
	ft_add_to_env(sack, keyval);
	return (free(keyval), free(nbrstr), tree_node->exit_code);
}

/*
	1. expand str (variables and wildcards if implemented)
	2. tokenize
	3. create cmd lst
	4. exec and wait
*/
int	ft_parse_and_exec(t_cmdtree *tree_node, t_mshell_sack *sack)
{
	int		status;

	while (ft_has_brackets(tree_node->cmd_str))
		ft_remove_outer_brackets(tree_node->cmd_str);
	tree_node->expanded_str = ft_strtrim(tree_node->cmd_str, " \t\n\r\v");
	tree_node->cmd_tokens = lexer(tree_node->expanded_str);
	ft_expand_list(tree_node->cmd_tokens, sack);
	if (!tree_node->cmd_tokens)
		ft_memory_err_exit(sack);
	status = ft_fill_cmdlist(tree_node->cmd_tokens, tree_node);
	if (!status)
		return (ft_exec_and_wait(tree_node, sack));
	if (status == 2)
		ft_memory_err_exit(sack);
	ft_putendl_fd("µShell: redirection error", STDERR_FILENO);
	sack->cont = 0;
	return (2);
}

char	*ft_remove_brackets(char *str)
{
	char	*res;

	res = ft_strdup(str);
	while (!get_log_expandible(res) && ft_has_brackets(res))
		ft_remove_outer_brackets(res);
	return (res);
}

int	logic_expansion(t_cmdtree *tree_node)
{
	char	*str;

	str = ft_remove_brackets(tree_node->cmd_str);
	tree_node->is_logic = get_log_expandible(str);
	if (tree_node->is_logic)
	{
		tree_node->left = ft_calloc(1, sizeof(t_cmdtree));
		tree_node->left->cmd_str = ft_get_left_token(str);
		tree_node->right = ft_calloc(1, sizeof(t_cmdtree));
		tree_node->right->cmd_str = ft_get_right_token(str);
		if ((!tree_node->left->cmd_str || !(*tree_node->left->cmd_str))
			|| (!tree_node->right->cmd_str || !(*tree_node->right->cmd_str)))
			return (2);
	}
	free(str);
	return (0);
}
