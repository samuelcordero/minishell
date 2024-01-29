/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:44:47 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/29 12:21:43 by sacorder         ###   ########.fr       */
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

static void	*get_next_token(char *str, int type, int *conts, int len)
{
	while (str[conts[0]] && !ft_isspace(str[conts[0]]))
		++(conts[0]);
	while (str[conts[0]] && conts[0] >= len)
		++(conts[0]);
	if (conts[0] >= len)
		ft_str_unquote(&str[len]);
	return (get_token(str, conts[1], conts[0], type));
}

static void	update_last_node(t_list **last_next, int *conts,
	int *lngths)
{
	t_list	*last;
	t_list	*next;

	last = last_next[0];
	next = last_next[1];
	if (last && last->next)
	{
		free(last->next);
		last->next = next;
		if (conts[0] > lngths[0])
			((t_cmdtkn *)last->content)->type = ARG;
	}
}

static	void	free_tmp(char *tmp)
{
	if (tmp)
		free(tmp);
}

int	retokenize(t_list *curr, int type, int start, int *lngths)
{
	int		conts[2];
	char	*tmp;
	t_list	*last_next[2];

	conts[0] = start;
	last_next[0] = NULL;
	last_next[1] = NULL;
	tmp = ft_strdup(((t_cmdtkn *)curr->content)->str);
	conts[1] = 0;
	last_next[1] = curr->next;
	while (tmp && tmp[conts[0]])
	{
		skip_spaces(tmp, &conts[0], &conts[1], 0);
		last_next[0] = curr;
		curr->content = get_next_token(tmp, type, conts, lngths[0]);
		if (!lngths[1])
			lngths[1] = conts[0];
		curr->next = ft_calloc(1, sizeof(t_list));
		if (!curr->content || !curr->next)
			return (1);
		skip_spaces(tmp, &conts[0], &conts[1], 1);
		curr = curr->next;
	}
	update_last_node(last_next, conts, lngths);
	return (free_tmp(tmp), 0);
}
