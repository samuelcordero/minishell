/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:03:18 by guortun-          #+#    #+#             */
/*   Updated: 2024/01/15 14:08:03 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

void	delim_subs(char **str, int j, char *tmp, char *tmp2)
{
	free(tmp);
	tmp = ft_strjoin(tmp2, " ");
	free(tmp2);
	tmp2 = ft_substr(*str, j, SIZE_T_MAX);
	free(*str);
	*str = ft_strjoin(tmp, tmp2);
	free(tmp);
	free(tmp2);
}

void	heredoc_management(char **line, char *prompt, char *delim, int *fd)
{
	while ((*line) && ft_strncmp((*line), delim,
			ft_strlen(delim) + 1))
	{
		ft_putendl_fd((*line), *fd);
		free((*line));
		(*line) = NULL;
		(*line) = readline(prompt);
	}
}
