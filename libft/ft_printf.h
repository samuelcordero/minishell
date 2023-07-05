/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 11:52:47 by sacorder          #+#    #+#             */
/*   Updated: 2023/04/30 18:44:37 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stddef.h>
# include <unistd.h>
# include <limits.h>

int	ft_handle_char(va_list lst);
int	ft_handle_str(va_list lst);
int	ft_handle_pointer(va_list lst);
int	ft_handle_unsigned(va_list lst, int type, char *base);
int	ft_handle_nbr_base(va_list lst, char *base);
int	ft_printulong_in_base(long long n, char *base);
int	ft_putnbr_base(long nbr, char *base);
int	ft_put_unsigned_nbr_base(unsigned long nbr, char *base, int pointer_type);
int	ft_printf(char const *str, ...);
#endif
