/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 20:11:15 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/02 19:57:32 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	state_quote_delimiter(char *str, int *i, char delim)
{
	++(*i);
	while (str[*i] != delim && str[*i])
		++(*i);
}

void	print_tokens(t_list *tokens)
{
	t_list		*current;
	t_cmdtoken	*ccontent;
	int			counter;

	current = tokens;
	counter = 0;
	while (current)
	{
		ccontent = (t_cmdtoken *)current->content;
		printf("Token %i\n\n", counter);
		if (ccontent)
			printf("Content: (%s) \nType: %i\n", ccontent->str, ccontent->type);
		printf("----------------------\n\n");
		counter++;
		current = current->next;
	}
}

void	skip_spaces(char *str, int *i, int *start)
{
	while (str[*i] == ' ' && str[*i])
		++(*i);
	*start = *i;
}

static int	ft_str_unquote(char **str)
{
	int	i;
	int	j;
	int	q_ctr;

	i = 0;
	q_ctr = 0;
	while (str && *str && (*str)[i])
	{
		if ((*str)[i] == '"' || (*str)[i] == '\'')
		{
			j = i;
			while ((*str)[j])
			{
				(*str)[j] = (*str)[j + 1];
				++j;
			}		
			++q_ctr;
		}
		else
			++i;
	}
	if (q_ctr % 2)
		return (1);
	return (0);
}

int	ft_remove_quotes(t_list *tokens)
{
	t_cmdtoken	*content;

	while (tokens)
	{
		content = tokens->content;
		if (ft_str_unquote(&(content->str)))
			return (1);
		tokens = tokens->next;
	}
	return (0);
}