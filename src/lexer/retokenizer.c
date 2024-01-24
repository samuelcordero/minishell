/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:44:47 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/24 12:12:30 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*get_token(char *str, int start, int end, int type)
{
	t_cmdtkn	*token;

	token = ft_calloc(1, sizeof(t_cmdtkn));
	if (!token)
		return (NULL);
	token->str = ft_substr(str, start, end - start);
	token->type = type;
	return ((void *) token);
}

static void	*get_next_token(char *str, int start, int *i, int type, int len)
{
	while (str[*i] && !ft_isspace(str[*i]))
		++(*i);
	while (str[*i] && *i >= len)
		++(*i);
	if (*i >= len)
		ft_str_unquote(&str[len]);
	return (get_token(str, start, *i, type));
}

int	retokenize(t_list *curr, int type, int start, int *new_len, int len)
{
	t_list	*last;
	int		i;
	int		tok_start;
	char	*tmp;
	t_list	*next;

	i = start;
	last = NULL;
	tmp = ft_strdup(((t_cmdtkn *)curr->content)->str);
	tok_start = 0;
	next = curr->next;
	while (tmp && tmp[i])
	{
		skip_spaces(tmp, &i, &tok_start, 0);
		last = curr;
		curr->content = get_next_token(tmp, tok_start, &i, type, len);
		if (!*new_len)
			*new_len = i;
		curr->next = ft_calloc(1, sizeof(t_list));
		if (!curr->content || !curr->next)
			return (1);
		skip_spaces(tmp, &i, &tok_start, 1);
		curr = curr->next;
	}
	if (last && last->next)
	{
		free(last->next);
		last->next = next;
		if (i > len && type == W_EXP_ARG)
			((t_cmdtkn *)last->content)->type = ARG;
	}
	if (tmp)
		free(tmp);
	return (0);
}
