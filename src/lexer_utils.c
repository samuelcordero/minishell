/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 20:11:15 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/01 21:02:44 by sacorder         ###   ########.fr       */
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
	int	counter;

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
