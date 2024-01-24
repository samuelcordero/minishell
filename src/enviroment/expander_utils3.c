/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 13:46:56 by guortun-          #+#    #+#             */
/*   Updated: 2024/01/24 13:48:07 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	double_quote_state(t_list *curr, int *i, t_mshell_sack *sack)
{
	int			j;
	t_cmdtoken	*tok;

	j = *i;
	tok = curr->content;
	while (tok->str[j + 1])
	{
		tok->str[j] = tok->str[j + 1];
		++j;
	}
	while (tok->str[*i] != '"' && tok->str[*i])
	{
		if (tok->str[*i] == '$')
			env_state(curr, i, 0, sack);
		++(*i);
	}
	j = *i;
	while (tok->str[j] && tok->str[j + 1])
	{
		tok->str[j] = tok->str[j + 1];
		++j;
	}
	tok->str[j - 1] = '\0';
}

void	single_quote_state(t_list *curr, int *i)
{
	int			j;
	t_cmdtoken	*tok;

	j = *i;
	tok = curr->content;
	while (tok->str[j + 1])
	{
		tok->str[j] = tok->str[j + 1];
		++j;
	}
	while (tok->str[*i] != '\'' && tok->str[*i])
		++(*i);
	j = *i;
	while (tok->str[j] && tok->str[j + 1])
	{
		tok->str[j] = tok->str[j + 1];
		++j;
	}
	tok->str[j - 1] = '\0';
}

void	expand_list(t_list *curr, t_mshell_sack *sack)
{
	t_cmdtoken	*tok;
	int			i;
	int			pre_type;

	tok = curr->content;
	i = 0;
	pre_type = tok->type;
	while (tok->str[i])
	{
		if (tok->str[i] == '\'' && pre_type == ARG)
			single_quote_state(curr, &i);
		else if (tok->str[i] == '"' && pre_type == ARG)
			double_quote_state(curr, &i, sack);
		else if (tok->str[i] == '*' && pre_type == E_EXP_ARG)
			return (wildcard_state(curr, &i, sack), (void)0);
		else if (tok->str[i] == '$' && pre_type == ARG)
			env_state(curr, &i, 1, sack);
		else
			++i;
		tok = curr->content;
	}
	if (pre_type == E_EXP_ARG)
		tok->type = W_EXP_ARG;
	if (pre_type == ARG)
		tok->type = E_EXP_ARG;
}
