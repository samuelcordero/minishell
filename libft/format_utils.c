/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 13:59:42 by sacorder          #+#    #+#             */
/*   Updated: 2023/04/30 19:25:32 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_handle_char(va_list lst)
{
	char	c;

	c = (char) va_arg(lst, int);
	return (write(1, &c, 1));
}

int	ft_handle_str(va_list lst)
{
	char	*str;
	int		counter;

	str = va_arg(lst, char *);
	counter = 0;
	if (str)
	{
		while (str[counter])
		{
			if (write(1, &str[counter], 1) == -1)
				return (-1);
			++counter;
		}
		return (counter);
	}
	if (write(1, "(null)", 6) == -1)
		return (-1);
	return (6);
}

int	ft_handle_pointer(va_list lst)
{
	unsigned long	nbr;
	int				res;

	nbr = va_arg(lst, unsigned long);
	if (write(1, "0x", 2) == -1)
		return (-1);
	res = ft_put_unsigned_nbr_base(nbr, "0123456789abcdef", 1);
	if (res == -1)
		return (res);
	return (2 + res);
}

int	ft_handle_unsigned(va_list lst, int type, char *base)
{
	unsigned long	nbr;

	nbr = va_arg(lst, unsigned int);
	if (type)
		return (ft_printulong_in_base(nbr, base));
	return (ft_put_unsigned_nbr_base(nbr, base, 0));
}

int	ft_handle_nbr_base(va_list lst, char *base)
{
	int	nbr;

	nbr = va_arg(lst, int);
	return (ft_putnbr_base(nbr, base));
}
