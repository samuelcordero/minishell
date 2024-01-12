/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/12 15:50:12 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_add_subtokens_env(char *str, t_list *list)
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
		ft_add_subtokens_env(tmp, list);
	}
	free(tmp);
}

static void	ft_add_subtokens_wcards(t_list *list, t_list *tmp)
{
	t_list	*ref;

	ref = list->next;
	while (tmp)
	{
		((t_cmdtoken *) list->content)->str = ft_strdup(((t_cmdtoken *) tmp->content)->str);
		((t_cmdtoken *) list->content)->type = ((t_cmdtoken *) tmp->content)->type;
		tmp = tmp->next;
		if (tmp)
		{
			list->next = ft_calloc(1, sizeof(t_list));
			list = list->next;
			list->content = ft_calloc(1, sizeof(t_cmdtoken));
		}
	}
	list->next = ref;
}

void	ft_expand_wcard_list(t_list *list, t_mshell_sack *sack)
{
	t_cmdtoken	*content;
	t_list		*tmp;
	(void) sack;
	
	content = list->content;
	tmp = ft_get_files(content->str);
	if (tmp)
	{
		ft_add_subtokens_wcards(list, tmp);
		ft_lstclear(&tmp, free_cmd_tok);
	}
}
