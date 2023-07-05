/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strnbr_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 16:37:58 by sacorder          #+#    #+#             */
/*   Updated: 2023/05/01 15:23:02 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		++i;
	return (i);
}

static int	ft_printpositivelong_in_base(long long n, char *base)
{
	long	basenbr;
	int		res;

	basenbr = ft_strlen(base);
	res = 0;
	if (n < 0)
		return (write(1, "Error\n", 6));
	if (n / basenbr == 0)
		return (write(1, &base[n], 1));
	res = ft_printpositivelong_in_base(n / basenbr, base);
	if (res == -1)
		return (-1);
	return (res + write(1, &base[n % basenbr], 1));
}

int	ft_printulong_in_base(long long n, char *base)
{
	unsigned long	basenbr;
	int				res;

	basenbr = ft_strlen(base);
	res = 0;
	if (n / basenbr == 0)
		return (write(1, &base[n], 1));
	res = ft_printpositivelong_in_base(n / basenbr, base);
	if (res == -1)
		return (-1);
	return (res + write(1, &base[n % basenbr], 1));
}

int	ft_putnbr_base(long nbr, char *base)
{
	long	nbrll;
	int		res;

	nbrll = nbr;
	res = 0;
	if (ft_strlen(base) >= 2)
	{
		if (nbrll < 0)
		{
			if (write(1, "-", 1) == -1)
				return (-1);
			nbrll *= -1;
			res = 1;
		}
		res += ft_printpositivelong_in_base(nbrll, base);
	}
	return (res);
}

int	ft_put_unsigned_nbr_base(unsigned long nbr, char *base, int pointer_type)
{
	int				res;

	res = 0;
	if (!pointer_type)
	{	
		if ((long) nbr == LONG_MIN)
		{
			if (write(1, "0", 1) == -1)
				return (-1);
			res = 1;
		}
		else if (ft_strlen(base) >= 2)
			res = ft_printulong_in_base(nbr, base);
	}
	else if (ft_strlen(base) >= 2)
		res = ft_printulong_in_base(nbr, base);
	return (res);
}
