/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:36:01 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/16 18:19:41 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmplst;
	t_list	*new_entry;
	t_list	*result_start;

	tmplst = lst;
	if (tmplst)
	{
		result_start = ft_lstnew(f(tmplst->content));
		if (!result_start)
			return (0);
		tmplst = tmplst->next;
		while (tmplst)
		{
			new_entry = ft_lstnew(f(tmplst->content));
			if (!new_entry)
			{
				ft_lstclear(&result_start, del);
				return (0);
			}
			ft_lstadd_back(&result_start, new_entry);
			tmplst = tmplst->next;
		}
		return (result_start);
	}
	return (0);
}
