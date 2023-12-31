/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 17:22:18 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/23 00:21:55 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen_gnl(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		++i;
	return (i);
}

static char	*ft_malloc_len(size_t len, char const *s, unsigned int start)
{
	size_t	slen;

	slen = ft_strlen_gnl(s);
	if (slen - start == 0)
		return (NULL);
	else if (start > slen)
		return ((char *) malloc(sizeof(char)));
	else if (slen - start < len)
		return ((char *) malloc(sizeof(char) * (slen - start + 1)));
	else if (slen - start >= len)
		return ((char *) malloc(sizeof(char) * (len + 1)));
	else
		return ((void *) 0);
}

char	*ft_strchr_gnl(const char*s, int c)
{
	if (!s)
		return ((void *) 0);
	while (*s)
	{
		if (*s == (char) c)
			return ((char *) s);
		++s;
	}
	if ((char) c == '\0')
		return ((char *) s);
	return ((void *) 0);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	pos_res;
	size_t	pos_s;

	pos_s = 0;
	pos_res = 0;
	res = ft_malloc_len(len, s, start);
	if (!res || !s)
		return (NULL);
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

char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	size_t	s1len;
	size_t	s2len;
	char	*res;
	size_t	pos;

	s1len = ft_strlen_gnl(s1);
	s2len = ft_strlen_gnl(s2);
	res = (char *) malloc(sizeof(char) * (s1len + s2len + 1));
	if (res == (void *) 0)
		return (res);
	pos = 0;
	while (pos < s1len)
	{
		res[pos] = s1[pos];
		++pos;
	}
	pos = 0;
	while (pos < s2len)
	{
		res[pos + s1len] = s2[pos];
		++pos;
	}
	res[pos + s1len] = '\0';
	return (res);
}
