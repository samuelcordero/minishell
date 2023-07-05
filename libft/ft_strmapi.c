/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 12:55:29 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/09 14:18:06 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static unsigned int	ft_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	if (str == (void *) 0)
		return (0);
	while (str[i] != '\0')
		++i;
	return (i);
}

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*result;
	unsigned int	position;

	position = 0;
	result = (char *) malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!result)
		return (result);
	while (s[position] != '\0')
	{
		result[position] = (*f)(position, s[position]);
		++position;
	}
	result[position] = '\0';
	return (result);
}
