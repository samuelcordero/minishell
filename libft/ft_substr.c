/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:07:28 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/13 16:50:42 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static char	*ft_malloc_len(size_t len, char const *s, unsigned int start)
{
	size_t	slen;

	slen = ft_strlen(s);
	if (start > slen)
		return ((char *) malloc(sizeof(char)));
	else if (slen - start < len)
		return ((char *) malloc(sizeof(char) * (slen - start + 1)));
	else if (slen - start >= len)
		return ((char *) malloc(sizeof(char) * (len + 1)));
	else
		return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	pos_res;
	size_t	pos_s;

	pos_s = 0;
	pos_res = 0;
	res = ft_malloc_len(len, s, start);
	if (!res || !s)
		return (res);
	if (len > 0)
	{
		while (s[pos_s] != '\0' && pos_res < len)
		{
			if (pos_s >= start)
			{
				res[pos_res] = s[pos_s];
				++pos_res;
			}
			++pos_s;
		}
	}
	res[pos_res] = '\0';
	return (res);
}
