/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 17:09:04 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/16 16:32:53 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static unsigned int	ft_isset(char const c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		++i;
	}
	return (0);
}

static size_t	ft_startpos(char const *s, char const *set)
{
	size_t	i;

	i = 0;
	while (ft_isset(s[i], set) && s[i] != '\0')
		++i;
	return (i);
}

static size_t	ft_endpos(char const *s, char const *set)
{
	size_t	i;

	i = ft_strlen(s) - 1;
	while (ft_isset(s[i], set) && i > 0)
		--i;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;

	if (!s1 || !set || !s1[0])
	{
		res = (char *) malloc(sizeof(char) * 1);
		res[0] = '\0';
		return (res);
	}
	else if (ft_startpos(s1, set) > ft_endpos(s1, set))
	{
		res = (char *) malloc(sizeof(char) * 1);
		res[0] = '\0';
		return (res);
	}
	res = ft_substr(s1, (unsigned int) ft_startpos(s1, set),
			ft_endpos(s1, set) - ft_startpos(s1, set) + 1);
	return (res);
}
