/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:41:30 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/24 12:41:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wildcard_state(t_list *curr, int *i, t_mshell_sack *sack)
{
	int			cont[2];
	t_cmdtkn	*tok;
	char		*regex;
	char		*files;
	char		*tmp;

	cont[0] = *i + 1;
	cont[1] = *i;
	tok = curr->content;
	while (cont[1] > 0 && !ft_isspace(tok->str[cont[1]]))
		--cont[1];
	while (ft_isspace(tok->str[cont[1]]))
		++cont[1];
	while (tok->str[cont[0]] && !ft_isspace(tok->str[cont[0]]))
	{
		if (tok->str[cont[0]] == '\'')
			single_quote_state(curr, &cont[0]);
		else if (tok->str[cont[0]] == '"')
			double_quote_state(curr, &cont[0], sack);
		else
			++cont[0];
	}
	cont[0] = *i;
	while (tok->str[cont[0]] && !ft_isspace(tok->str[cont[0]]))
		++cont[0];
	regex = ft_substr(tok->str, cont[1], cont[0] - cont[1]);
	files = ft_get_files(regex);
	free(regex);
	if (!files)
		return (tok->type = W_EXP_ARG, (void) 0);
	tmp = ft_substr(tok->str, 0, cont[1]);
	regex = ft_strjoin(tmp, files);
	free(files);
	files = ft_strjoin(regex, &(tok->str[cont[0]]));
	free(regex);
	free(tok->str);
	tok->str = files;
	retokenize(curr, W_EXP_ARG, cont[1], &cont[0], ft_strlen(files) + *i);
	if (tok != curr->content)
		free_cmd_tok(tok);
}

void	double_quote_state(t_list *curr, int *i, t_mshell_sack *sack)
{
	int			j;
	t_cmdtkn	*tok;

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
	t_cmdtkn	*tok;

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
