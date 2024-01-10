/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 13:07:54 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/10 14:29:05 by sacorder         ###   ########.fr       */
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

void	ft_memory_err_exit(t_mshell_sack *sack)
{
	ft_putendl_fd("minishell: memory error", STDERR_FILENO);
	ft_printexit(2, sack, 0);
}

void	tmp_management(char **tmp, int *shllvl)
{
	ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
	*tmp = ft_itoa(*shllvl);
	ft_putstr_fd(*tmp, STDERR_FILENO);
	free(*tmp);
	*tmp = NULL;
	ft_putendl_fd(") too high, resetting to 1", STDERR_FILENO);
	*shllvl = 1;
}

int	ft_check_f_name(char *str, int *i)
{
	int starting[2];

	starting[0] = *i;
	if (!ft_strncmp("<<", &str[*i], 2) || !ft_strncmp(">>", &str[*i], 2))
		*i += 2;
	else if (!ft_strncmp("<", &str[*i], 1) || !ft_strncmp(">", &str[*i], 1))
		++(*i);
	while (str[*i] && ft_isspace(str[*i]))
		++(*i);
	starting[1] = *i;
	while (str[*i] && !ft_isreserved(str[*i]) && !ft_isspace(str[*i]))
		++(*i);
	if (starting[1] != *i)
		return (0);
	ft_putstr_fd("minishell: syntax error near token '", STDERR_FILENO);
	if (!ft_strncmp("<<", &str[starting[0]], 2) || !ft_strncmp(">>", &str[starting[0]], 2))
		write(STDERR_FILENO, &str[starting[0]], 2);
	else if (!ft_strncmp("<", &str[starting[0]], 1) || !ft_strncmp(">", &str[starting[0]], 1))
		write(STDERR_FILENO, &str[starting[0]], 1);
	ft_putendl_fd("'", STDERR_FILENO);
	return (1);
}
