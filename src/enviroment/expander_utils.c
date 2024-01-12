/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/12 14:41:43 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	ft_count_subtokens(char *str, char **envp)
{
	t_list	*list;
	char	*tmp;
	int		res;

	tmp = ft_expand(str, envp, 0);
	list = lexer(tmp);
	res = ft_lstsize(list);
	ft_lstclear(&list, free_cmd_tok);
	free(tmp);
	return (res);
} */

static void	ft_add_subtokens(char *str, t_list *list)
{
	t_list	*tmp_list;
	t_list	*iter;
	t_list	*ref;

	tmp_list = lexer(str);
	iter = tmp_list;
	ref = list->next;
	while (iter && list)
	{
		if (iter->content && ((t_cmdtoken *)iter->content)->str)
			((t_cmdtoken *)list->content)->str = ft_strdup(((t_cmdtoken *)iter->content)->str);
		else
			((t_cmdtoken *)list->content)->str = ft_strdup("");
		((t_cmdtoken *)list->content)->type = EXP_ARG;
		iter = iter->next;
		if (iter)
		{
			list->next = ft_calloc(1, sizeof(t_list));
			list->next->content = ft_calloc(1, sizeof(t_cmdtoken));
			list = list->next;
		}
	}
	list->next = ref;
	ft_lstclear(&tmp_list, free_cmd_tok);
}

void	ft_expand_env_list(t_list *list, t_mshell_sack *sack)
{
	t_cmdtoken	*content;
	char		*tmp;
	
	content = list->content;
	tmp = ft_expand(content->str, sack->envp, 0);
	if (tmp)
	{
		free(content->str);
		ft_add_subtokens(tmp, list);
	}
	free(tmp);
}

void	ft_expand_wcard_list(t_list *list, t_mshell_sack *sack)
{
	(void) list;
	(void) sack;
/* 	t_list	*tmp_list;
	t_list	*iter;
	t_list	*ref;
	char	*tmp;

	tmp_list = lexer(str);
	iter = tmp_list;
	ref = list->next;
	while (iter && list)
	{
		if (iter->content && ((t_cmdtoken *)iter->content)->str)
			((t_cmdtoken *)list->content)->str = ft_strdup(((t_cmdtoken *)iter->content)->str);
		else
			((t_cmdtoken *)list->content)->str = ft_strdup("");
		((t_cmdtoken *)list->content)->type = EXP_ARG;
		iter = iter->next;
		if (iter)
		{
			list->next = ft_calloc(1, sizeof(t_list));
			list->next->content = ft_calloc(1, sizeof(t_cmdtoken));
			list = list->next;
		}
	}
	list->next = ref;
	ft_lstclear(&tmp_list, free_cmd_tok); */
}
