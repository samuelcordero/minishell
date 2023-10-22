/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 13:47:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/05/19 20:25:53 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	pos;
	size_t	j;

	i = 0;
	if (!needle[0])
		return ((char *) haystack);
	while (haystack[i] != '\0' && i < len)
	{
		if (haystack[i] == needle[0])
		{
			pos = i;
			j = 0;
			while (haystack[pos] == needle[j] && needle[j] && haystack[pos])
			{
				++pos;
				++j;
			}
			if (needle[j] == '\0' && pos <= len)
				return ((char *) &haystack[i]);
		}
		++i;
	}
	return ((void *) 0);
}
