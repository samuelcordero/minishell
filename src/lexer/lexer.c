/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:44:47 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 12:14:07 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*get_token(char *str, int start, int end)
{
	t_cmdtoken	*token;

	token = ft_calloc(1, sizeof(t_cmdtoken));
	if (!token)
		return (NULL);
	token->str = ft_substr(str, start, end - start);
	token->type = 0;
	if (!ft_strncmp(token->str, "<", 2) || !ft_strncmp(token->str, "<<", 3)
		|| !ft_strncmp(token->str, ">", 2) || !ft_strncmp(token->str, ">>", 3))
		token->type = 1;
	else if (!ft_strncmp(token->str, "|", 2))
		token->type = 2;
	else if (!ft_strncmp(token->str, "&&", 3)
		|| !ft_strncmp(token->str, "||", 3) || !ft_strncmp(token->str, ";", 2))
		token->type = 3;
	return ((void *) token);
}

char	ft_isreserved(char c)
{
	if (c != '|' && c != ';' && c != '<' && c != '>' && c != '&')
		return (0);
	return (1);
}

static void	*get_next_token(char *str, int start, int *i)
{
	while (str[*i] && !ft_isspace(str[*i]))
	{
		if (str[*i] == '\'')
			state_quote_delimiter(str, i, '\'');
		else if (str[*i] == '\"')
			state_quote_delimiter(str, i, '\"');
		if (ft_isreserved(str[*i]))
			break ;
		if (str[*i] && !ft_isspace(str[*i]))
			++(*i);
	}
	if (ft_isreserved(str[*i]) && start == *i)
	{
		++(*i);
		if (str[*i] == str[(*i) - 1])
			(*i) += 1;
	}
	return (get_token(str, start, *i));
}

static int	tokener(char *str, t_list *list)
{
	t_list	*current;
	t_list	*last;
	int		i;
	int		start;

	i = 0;
	current = list;
	last = NULL;
	while (str[i])
	{
		skip_spaces(str, &i, & start);
		last = current;
		current->content = get_next_token(str, start, &i);
		current->next = ft_calloc(1, sizeof(t_list));
		if (!current->content || !current->next)
			return (1);
		current = current->next;
	}
	if (last)
	{
		free(last->next);
		last->next = NULL;
	}
	return (0);
}

t_list	*lexer(char	*str)
{
	t_list	*list;

	list = ft_calloc(1, sizeof(t_list));
	if (!list)
		return (NULL);
	if (tokener(str, list))
		return (ft_lstclear(&list, free_cmd_tok), NULL);
	return (list);
}
