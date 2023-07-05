/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 13:15:42 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/15 18:29:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char*s, int c)
{
	while (*s)
	{
		if (*s == (char) c)
			return ((char *) s);
		++s;
	}
	if ((char) c == '\0')
		return ((char *) s);
	return (0);
}
