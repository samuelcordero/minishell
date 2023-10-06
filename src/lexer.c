/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:44:47 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/05 17:24:16 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*get_token(char *str, int start, int end)
{
	t_cmdtoken	*token;

	token = malloc(sizeof(t_cmdtoken));
	//if (!token) memory error handling
	token->str = ft_substr(str, start, end - start);
	token->type = 0;
	if (!ft_strncmp(token->str, "|", 2))
		token->type = 1;
	else if (!ft_strncmp(token->str, "<", 2) || !ft_strncmp(token->str, "<<", 3)
		|| !ft_strncmp(token->str, ">", 2) || !ft_strncmp(token->str, ">>", 3))
		token->type = 2;
	else if (!ft_strncmp(token->str, "&&", 3) || !ft_strncmp(token->str, "||", 3)
		|| !ft_strncmp(token->str, ";", 2))
		token->type = 3;
	return ((void *) token);
}

static void	skip_spaces(char *str, int *i, int *start)
{
	while (str[*i] == ' ' && str[*i])
		++(*i);
	*start = *i;
}

static char	ft_isreserved(char c)
{
	if (c != '|' && c != ';' && c != '<' && c != '>' && c != '&')
		return (0);
	return (1);
}

static void	*get_next_token(char *str, int start, int *i)
{
	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '\'')
			state_quote_delimiter(str, i, '\'');
		else if (str[*i] == '\"')
			state_quote_delimiter(str, i, '\"');
		if (ft_isreserved(str[*i]))
			break ;
		if (str[*i] && str[*i] != ' ')
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

static void	tokener(char *str, t_list *list)
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
		//IF current->content = NULL error
		current->next = malloc(sizeof(t_list));
		//if (!current->next) memory error
		current = current->next;
		current->content = NULL;
		current->next = NULL;
	}
	if (last)
	{
		free(last->next);
		last->next = NULL;
	}
}

t_list	*lexer(char	*str)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = NULL;
	list->next = NULL;
	tokener(str, list);
	return (list);
}
