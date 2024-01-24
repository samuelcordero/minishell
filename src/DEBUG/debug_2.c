/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 15:37:56 by guortun-          #+#    #+#             */
/*   Updated: 2024/01/23 20:15:54 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_variable(const char *varname, void *var, char type)
{
	char	variable;
	char	*variable_ptr;
	int		variable_int;

	variable = *(char *)var;
	variable_ptr = var;
	variable_int = *(int *)var;
	if (type == 1)
		printf("%s%s%s = %i\n", GREEN, varname, RST, (int)variable);
	else if (type == 2)
		printf("%s%s%s = %s\n", GREEN, varname, RST, variable_ptr);
	else if (type == 3)
		printf("%s%s%s = %d\n", GREEN, varname, RST, variable_int);
}
