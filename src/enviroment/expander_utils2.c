/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:41:30 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/24 13:48:53 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wildcard_state(t_list *curr, int *i, t_mshell_sack *sack)
{
	int			cont[3];
	t_cmdtkn	*tok;
	char		*regex;
	char		*files;
	char		*tmp;

	cont[1] = *i + 1;
	cont[2] = *i;
	tok = curr->content;
	while (cont[2] > 0 && !ft_isspace(tok->str[cont[2]]))
		--cont[2];
	while (ft_isspace(tok->str[cont[2]]))
		++cont[2];
	while (tok->str[cont[1]] && !ft_isspace(tok->str[cont[1]]))
	{
		if (tok->str[cont[1]] == '\'')
			single_quote_state(curr, &cont[1]);
		else if (tok->str[cont[1]] == '"')
			double_quote_state(curr, &cont[1], sack);
		else
			++cont[1];
	}
	cont[1] = *i;
	while (tok->str[cont[1]] && !ft_isspace(tok->str[cont[1]]))
		++cont[1];
	regex = ft_substr(tok->str, cont[2], cont[1] - cont[2]);
	files = ft_get_files(regex);
	free(regex);
	if (!files)
		return (tok->type = W_EXP_ARG, (void) 0);
	tmp = ft_substr(tok->str, 0, cont[2]);
	regex = ft_strjoin(tmp, files);
	free(files);
	files = ft_strjoin(regex, &(tok->str[cont[1]]));
	free(regex);
	free(tok->str);
	tok->str = files;
	cont[0] = ft_strlen(files) + *i;
	retokenize(curr, W_EXP_ARG, cont[2], cont);
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
