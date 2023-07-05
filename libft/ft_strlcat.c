/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 12:43:36 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/10 14:07:59 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dstlen;

	i = 0;
	j = 0;
	if (dstsize == 0)
		return (ft_strlen(src));
	dstlen = ft_strlen(dst);
	i = dstlen;
	if (dstsize < dstlen)
		return (ft_strlen(src) + dstsize);
	while ((i < dstsize - 1) && src[j] != '\0')
	{
		dst[i] = src[j];
		++i;
		++j;
	}
	dst[i] = '\0';
	return (ft_strlen(src) + dstlen);
}
