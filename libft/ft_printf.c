/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:50:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/04/30 19:51:29 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_handler(va_list lst, char escape)
{
	if (escape == 'c')
		return (ft_handle_char(lst));
	else if (escape == 's')
		return (ft_handle_str(lst));
	else if (escape == 'p')
		return (ft_handle_pointer(lst));
	else if (escape == 'd' || escape == 'i')
		return (ft_handle_nbr_base(lst, "0123456789"));
	else if (escape == 'u')
		return (ft_handle_unsigned(lst, 1, "0123456789"));
	else if (escape == 'x')
		return (ft_handle_unsigned(lst, 0, "0123456789abcdef"));
	else if (escape == 'X')
		return (ft_handle_unsigned(lst, 0, "0123456789ABCDEF"));
	else if (escape == '%')
		return (write(1, &escape, 1));
	return (-1);
}

static int	print_loop(char const *str, va_list lst)
{
	int		read;
	int		chars;
	long	position;

	position = -1;
	chars = 0;
	while (str[++position])
	{
		if (str[position] == '%')
		{
			++position;
			read = ft_handler(lst, str[position]);
			if (read == -1)
				return (read);
			chars += read;
		}
		else
		{
			if (write(1, &str[position], 1) == -1)
				return (-1);
			++chars;
		}
	}
	return (chars);
}

int	ft_printf(char const *str, ...)
{
	int		chars;
	va_list	lst;

	va_start(lst, str);
	chars = print_loop(str, lst);
	return (va_end(lst), chars);
}
