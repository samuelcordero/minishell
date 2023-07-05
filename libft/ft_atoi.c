/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 13:49:40 by sacorder          #+#    #+#             */
/*   Updated: 2023/03/16 18:38:18 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_is_space(char c)
{
	if (c == ' ' || c == '\v' || c == '\r' || c == '\t' || c == '\f'
		|| c == '\n')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	n;
	int	i;

	n = 0;
	sign = 1;
	i = 0;
	while (ft_is_space(str[i]))
		++i;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		++i;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		n *= 10;
		n += str[i] - 48;
		++i;
	}
	return (n * sign);
}
