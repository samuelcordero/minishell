/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:31:34 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/27 13:30:30 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_printexit(int exit_code)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	exit(exit_code);
}

void	ft_free_array(char **array)
{
	int	pos;

	pos = 0;
	if (!array)
		exit(-1);
	while (array[pos])
		free(array[pos++]);
	free(array);
}

void	free_cmd_tok(void *tok)
{
	t_cmdtoken	*tofree;

	tofree = tok;
	free(tofree->str);
	free(tofree);
}

char	*get_cwd_str(void)
{
	void	*nullable;
	char	cwd[256];

	nullable = getcwd(cwd, sizeof(cwd));
	if (!nullable)
	{
		perror("MiniShell: update cwd error");
		return (NULL);
	}
	return (ft_strdup(cwd));
}
