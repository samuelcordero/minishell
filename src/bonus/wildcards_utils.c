/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:15:01 by guortun-          #+#    #+#             */
/*   Updated: 2023/12/28 12:15:19 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_char_unop(char token, int index, char unop)
{
	if (token != '\0')
	{
		if (token == '+')
			return (index + 1);
		if (token == '-')
			return (index - 1);
	}
	return (index);
}

int	strlen_and_free(char **tmp, char **tmp2, char **file)
{
	int	i;

	*tmp = *tmp2;
	if (*file)
	{
		i = (1 + ft_strlen(*file));
		free(*file);
		return (i);
	}
	else
		return (-1);
}
