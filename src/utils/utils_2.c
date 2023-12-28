/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 13:07:54 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 12:45:22 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_set_signal_print(int val)
{
	struct termios	settings;

	if (tcgetattr(STDIN_FILENO, &settings) == -1)
	{
		perror("tcgetattr");
		return (1);
	}
	if (val)
		settings.c_lflag |= ECHOCTL;
	else
		settings.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &settings) == -1)
	{
		perror("tcsetattr");
		return (1);
	}
	return (0);
}

void ft_memory_err_exit(t_mshell_sack *sack)
{
	ft_putendl_fd("minishell: memory error", STDERR_FILENO);
	ft_printexit(2, sack, 0);
}