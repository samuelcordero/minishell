/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/31 19:23:12 by sacorder         ###   ########.fr       */
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

static char	*ft_join_env(t_cmdtkn *tok, int j, char **tmp, t_mshell_sack *sack)
{
	tmp[2] = ft_strjoin(tmp[1], ft_get_from_env(sack->envp, tmp[0], NULL));
	free(tmp[0]);
	free(tmp[1]);
	tmp[1] = ft_substr(tok->str, j + 1, SIZE_T_MAX);
	tmp[0] = ft_strjoin(tmp[2], tmp[1]);
	free(tmp[1]);
	free(tmp[2]);
	free(tok->str);
	return (tmp[0]);
}

void	env_state(t_list *curr, int *i, int check_w_cards, t_mshell_sack *sack)
{
	int			j;
	t_cmdtkn	*tok;
	int			lengths[2];
	char		*tmp[3];

	j = *i;
	lengths[1] = 0;
	tok = curr->content;
	while (tok->str[j + 1] && tok->str[j + 1] != '"' && tok->str[j + 1] != '$'
		&& !ft_isreserved(tok->str[j + 1]) && tok->str[j + 1] != '\''
		&& tok->str[j + 1] != '*' && !ft_isspace(tok->str[j + 1]))
		++j;
	tmp[0] = ft_substr(tok->str, *i + 1, j - *i);
	if (!ft_strncmp(tmp[0], "", 1))
		return (++(*i), free(tmp[0]), (void) 0);
	lengths[0] = ft_strlen(ft_get_from_env(sack->envp, tmp[0], NULL)) + *i;
	tmp[1] = ft_substr(tok->str, 0, *i);
	tok->str = ft_join_env(tok, j, tmp, sack);
	if (check_w_cards)
		retokenize(curr, E_EXP_ARG, *i, lengths);
	else if (lengths[0] != *i)
		*i = lengths[0];
	if (tok != curr->content)
		free_cmd_tok(tok);
	((t_cmdtkn *) curr->content)->type = E_EXP_ARG;
}
