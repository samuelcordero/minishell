/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:44:47 by sacorder          #+#    #+#             */
/*   Updated: 2023/07/05 20:29:19 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	*get_token(char *str, int start, int end)
{
	t_cmdtoken	*token;

	token = malloc(sizeof(t_cmdtoken));
	//if (!token) memory error handling
	token->str = ft_substr(str, start, end - start);
	token->type = 0;
	return ((void *) token);
}

static void	skip_spaces(char *str, int *i, int *start)
{
	while (str[*i] == ' ' && str[*i])
		++(*i);
	*start = *i;
}

static void	*get_next_token(char *str, int start, int *i)
{
	int		exit;

	exit = 0;
	while (str[*i] && !exit)
	{
		if (str[*i] == ' ')
			exit = 1;
		else if (str[*i] == '\'')
			state_quote_delimiter(str, i, '\'');
		else if (str[*i] == '\"')
			state_quote_delimiter(str, i, '\"');
		if (str[*i] && str[*i] != ' ')
			++(*i);
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
