/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 13:07:12 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/16 16:24:51 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static long	ft_intsize(int n)
{
	unsigned int	size;

	size = 1;
	while (n / 10 != 0)
	{
		n /= 10;
		++size;
	}
	return (size);
}

static long	ft_limit_putsign(long *ntmp, char *res)
{
	if (*ntmp >= 0)
		return (0);
	*ntmp *= -1;
	res[0] = '-';
	return (1);
}

char	*ft_itoa(int n)
{
	char	*result;
	size_t	size;
	size_t	limit;
	long	ntmp;

	size = ft_intsize(n);
	limit = 0;
	ntmp = n;
	if (ntmp < 0)
		size += 1;
	result = (char *) malloc(sizeof(char) * size + 1);
	if (!result)
		return (result);
	limit = ft_limit_putsign(&ntmp, result);
	result[size] = '\0';
	while (size > limit)
	{
		result[size - 1] = (ntmp % 10) + '0';
		ntmp /= 10;
		--size;
	}
	return (result);
}
