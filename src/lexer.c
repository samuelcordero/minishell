/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:44:47 by sacorder          #+#    #+#             */
/*   Updated: 2023/07/05 17:24:46 by sacorder         ###   ########.fr       */
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

static void	tokener(char *str, t_list *list)
{
	t_list	*current;
	int		i;
	int		start;
	//int		state;

	i = 0;
	current = list;
	//state = 0;
	while (str[i])
	{
		start = i;
		while (str[i] != ' ' && str[i])
			++i;
		if (start != i)
		{
			current->content = get_token(str, start, i);
			current->next = malloc(sizeof(t_list));
			//if (!current->next) memory error
			current = current->next;
			current->content = NULL;
			current->next = NULL;
		}
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

void	print_tokens(t_list *tokens)
{
	t_list		*current;
	t_cmdtoken	*ccontent;
	int	counter;

	current = tokens;
	counter = 0;
	while (current)
	{
		ccontent = (t_cmdtoken *)current->content;
		printf("Token %i\n\n", counter);
		if (ccontent)
			printf("Content: %s \nType: %i\n\n", ccontent->str, ccontent->type);
		counter++;
		current = current->next;
	}
}
