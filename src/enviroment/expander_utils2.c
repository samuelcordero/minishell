/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:41:30 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/19 18:39:54 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wildcard_state(t_list *curr, int *i, t_mshell_sack *sack)
{
	int			j;
	int			k;
	t_cmdtoken	*tok;
	char		*regex;
	char		*files;
	char		*tmp;

	j = *i + 1;
	k = *i;
	tok = curr->content;
	while (k > 0 && !ft_isspace(tok->str[k]))
		--k;
	while (ft_isspace(tok->str[k]))
		++k;
	while (tok->str[j] && !ft_isspace(tok->str[j]))
	{
		if (tok->str[j] == '\'')
			single_quote_state(curr, &j);
		else if (tok->str[j] == '"')
			double_quote_state(curr, &j, sack);
		else
			++j;
	}
	j = *i;
	while (tok->str[j] && !ft_isspace(tok->str[j]))
		++j;
	regex = ft_substr(tok->str, k, j - k);
	DEBUG_STR(regex);
	files = ft_get_files(regex);
	free(regex);
	if (!files)
		return (tok->type = W_EXP_ARG, (void) 0);
	tmp = ft_substr(tok->str, 0, k);
	regex = ft_strjoin(tmp, files);
	free(files);
	files = ft_strjoin(regex, &(tok->str[j]));
	free(tok->str);
	tok->str = files;
	retokenize(curr, W_EXP_ARG, k, &j, ft_strlen(files) + *i);
}

void	env_state(t_list *curr, int *i, int check_w_cards, t_mshell_sack *sack)
{
	int			j;
	t_cmdtoken	*tok;
	char		*tmp[3];
	int			len;
	int			len_tmp;

	j = *i + 1;
	tok = curr->content;
	while (tok->str[j] && !ft_isspace(tok->str[j]) && tok->str[j] != '"' && tok->str[j] != '*'
			&& !ft_isreserved(tok->str[j]) && tok->str[j] != '\'' && tok->str[j] != '$')
		++j;
	if (j == *i + 1 && tok->str[j] == '$')
		++j;
	tmp[0] = ft_substr(tok->str, *i + 1, j - (*i + 1));
	len = ft_strlen(ft_get_from_env(sack->envp, tmp[0], NULL)) + *i;
	tmp[1] = ft_substr(tok->str, 0, *i);
	tmp[2] = ft_strjoin(tmp[1], ft_get_from_env(sack->envp, tmp[0], NULL));
	free(tmp[0]);
	free(tmp[1]);
	tmp[1] = ft_substr(tok->str, j, SIZE_T_MAX);
	tmp[0] = ft_strjoin(tmp[2], tmp[1]);
	free(tmp[1]);
	free(tmp[2]);
	free(tok->str);
	ft_printf("now is (%s)", tmp[0]);
	tok->str = tmp[0];
	if (check_w_cards)
		retokenize(curr, E_EXP_ARG, *i, &len_tmp, len);
	if (tok != curr->content)
		free_cmd_tok(tok);
	tok = curr->content;
	tok->type = E_EXP_ARG;
}

void	double_quote_state(t_list *curr, int *i, t_mshell_sack *sack)
{
	int			j;
	t_cmdtoken	*tok;

	j = *i;
	tok = curr->content;
	while (tok->str[j + 1]) //quita la primera comilla y mueve todo el string a la izquierda
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
	while (tok->str[j] && tok->str[j + 1]) //mueve el string a la izquierda desde la segunda comilla simple
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
	while (tok->str[j + 1]) //quita la primera comilla y mueve todo el string a la izquierda
	{
		tok->str[j] = tok->str[j + 1];
		++j;
	}
	while (tok->str[*i] != '\'' && tok->str[*i])
		++(*i);
	j = *i;
	while (tok->str[j] && tok->str[j + 1]) //mueve el string a la izquierda desde la segunda comilla simple
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
