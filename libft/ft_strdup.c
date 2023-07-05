/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:53:03 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/07 16:35:03 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*res;

	i = 0;
	res = (void *) 0;
	while (s1[i] != '\0')
		++i;
	res = (char *)malloc(i + 1);
	if (res == (void *) 0)
	{
		errno = ENOENT;
		return (res);
	}
	i = 0;
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		++i;
	}
	res[i] = '\0';
	return (res);
}
