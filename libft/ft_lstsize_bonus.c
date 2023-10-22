/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:14:00 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/16 18:20:21 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		size;
	t_list	*tmplst;

	size = 0;
	tmplst = lst;
	while (tmplst)
	{
		tmplst = tmplst->next;
		++size;
	}
	return (size);
}
