/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 19:17:16 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/22 15:26:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_history_file(t_mshell_sack *sack)
{
	char	*home;

	home = ft_get_from_env(sack->envp, "HOME", NULL);
	if (!*home)
	{
		sack->history_fd = -1;
		ft_putendl_fd(HIST_ERROR_MSG, STDERR_FILENO);
		return ;
	}
	home = ft_strjoin(home, HIST_TMP_BASE);
	if (!home)
	{
		ft_putendl_fd("Minishell: memory error.", STDERR_FILENO);
		exit(1);
	}
	sack->history_fd = open(home, O_CREAT | O_RDWR, 0644);
	free(home);
}

void	ft_fill_history(t_mshell_sack *sack)
{
	char	*line;
	char	*trim;

	line = get_next_line(sack->history_fd);
	while (line)
	{
		trim = ft_strtrim(line, "\n");
		if (!trim)
		{
			ft_putendl_fd("Minishell: memory error", STDERR_FILENO);
			exit(1);
		}
		add_history(trim);
		free(line);
		free(trim);
		line = get_next_line(sack->history_fd);
	}
}

void	ft_add_history(char *str, t_mshell_sack *sack)
{
	ft_putendl_fd(str, sack->history_fd);
	add_history(str);
}
