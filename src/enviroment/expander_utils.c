/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/29 12:24:28 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_list(t_list *curr, t_mshell_sack *sack)
{
	t_cmdtkn	*tok;
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

static char	*ft_join_env(t_cmdtkn *tok, int j, char **tmp, int *i)
{
	free(tmp[0]);
	free(tmp[1]);
	tmp[1] = ft_substr(tok->str, j, SIZE_T_MAX);
	tmp[0] = ft_strjoin(tmp[2], tmp[1]);
	free(tmp[1]);
	free(tmp[2]);
	free(tok->str);
	++(*i);
	return (tmp[0]);
}

void	env_state(t_list *curr, int *i, int check_w_cards, t_mshell_sack *sack)
{
	int			j;
	t_cmdtkn	*tok;
	int			lengths[2];
	char		*tmp[3];

	j = *i + 1;
	lengths[1] = 0;
	tok = curr->content;
	while (tok->str[j] && !ft_isspace(*tok->str) && tok->str[j] != '"'
		&& !ft_isreserved(tok->str[j]) && tok->str[j] != '\''
		&& tok->str[j] != '$' && tok->str[j] != '*')
		++j;
	if (j == *i + 1 && tok->str[j] == '$')
		++j;
	tmp[0] = ft_substr(tok->str, *i + 1, j - (*i + 1));
	lengths[0] = ft_strlen(ft_get_from_env(sack->envp, tmp[0], NULL)) + *i;
	tmp[1] = ft_substr(tok->str, 0, *i);
	tmp[2] = ft_strjoin(tmp[1], ft_get_from_env(sack->envp, tmp[0], NULL));
	tok->str = ft_join_env(tok, j, tmp, i);
	if (check_w_cards)
		retokenize(curr, E_EXP_ARG, *i, lengths);
	if (tok != curr->content)
		free_cmd_tok(tok);
	tok = curr->content;
	tok->type = E_EXP_ARG;
}
