/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 13:22:27 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/09 16:39:56 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	last;

	i = 0;
	last = -1;
	while (s[i] != '\0')
	{
		if (s[i] == (char) c)
			last = i;
		++i;
	}
	if (last >= 0)
		return ((char *) &s[last]);
	else if ((char) c == '\0')
		return ((char *) &s[i]);
	else
		return ((void *) 0);
}
