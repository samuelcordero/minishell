/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 16:31:36 by guortun-          #+#    #+#             */
/*   Updated: 2024/02/01 01:05:52 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delimit_cmp(char *str, int *j, int *b_ctr)
{
	if (str[*j] == '(')
		++(*b_ctr);
	else if (str[*j] == ')')
		--(*b_ctr);
	else if (str[*j] == '\'')
		state_quote_delimiter(str, j, '\'');
}

void	ft_remove_outer_brackets(char *str)
{
	int	i;
	int	j;
	int	b_ctr;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '(')
		{
			j = i;
			b_ctr = 1;
			while (b_ctr && str[++j])
				delimit_cmp(str, &j, &b_ctr);
			while (str[++j - 1])
				str[j - 1] = str[j];
			while (str[++i - 1])
				str[i - 1] = str[i];
			return ;
		}
		else if (str[i] == '\'')
			state_quote_delimiter(str, &i, '\'');
	}
}
/*
	Returns 0 if str is not logically expandable, else pos value
*/

int	get_log_expandible(char *str)
{
	int	i;
	int	last;

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
		if ((str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '|' && str[i + 1] == '|') || (str[i] == ';'))
			last = i++;
		else if (str[i] == '(')
			ft_brackets(str, &i);
		else if (str[i])
			++i;
	}
	return (if_mask(str, last));
}

char	ft_has_brackets(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'')
			state_quote_delimiter(str, &i, '\'');
		else if (str[i] == '"')
			state_quote_delimiter(str, &i, '"');
		else if (str[i] == '(')
			return (str[i]);
	}
	return (0);
}

void	ft_expansion_error(t_cmdtree *tree_node)
{
	ft_putstr_fd("µShell: syntax error near '", STDERR_FILENO);
	if (tree_node->is_logic == AND_MASK)
		ft_putstr_fd("&&", STDERR_FILENO);
	if (tree_node->is_logic == OR_MASK)
		ft_putstr_fd("||", STDERR_FILENO);
	if (tree_node->is_logic == WAIT_MASK)
		ft_putstr_fd(";", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}
