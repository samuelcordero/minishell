/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:59:26 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/11 13:18:09 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_subtokens(char *str, char **envp)
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
}

static void	ft_add_subtokens(char **arr, int *pos, char *str, char **envp)
{
	t_list	*list;
	t_list	*iter;
	char	*tmp;

	tmp = ft_expand(str, envp, 0);
	list = lexer(tmp);
	free(tmp);
	iter = list;
	while (iter)
	{
		arr[(*pos)++] = ft_strdup(((t_cmdtoken *)iter->content)->str);
		iter = iter->next;
	}
	ft_lstclear(&list, free_cmd_tok);
}

char	**ft_expand_arg_arr(char **args, char **envp)
{
	int		ctr;
	int		aux;
	char	**res;

	ctr = -1;
	aux = 0;
	while (args[++ctr])
		if (ft_strchr(args[ctr], '$'))
			aux += ft_count_subtokens(args[ctr], envp);
	aux += ctr;
	res = ft_calloc(aux + 1, sizeof(char *));
	if (!res)
		return (res);
	ctr = -1;
	aux = 0;
	while (args[++ctr])
	{
		if (ft_strchr(args[ctr], '$'))
			ft_add_subtokens(res, &aux, args[ctr], envp);
		else
			res[aux++] = ft_strdup(args[ctr]);
	}
	ft_free_array(args);
	return (res);
}
