/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:31:34 by sacorder          #+#    #+#             */
/*   Updated: 2024/02/01 01:05:52 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Prints EXIT, frees allocated mem and exits with provided exitcode
*/
void	ft_printexit(int exit_code, t_mshell_sack *sack, char print)
{
	if (print)
		ft_putendl_fd("exit", STDERR_FILENO);
	close(sack->history_fd);
	free(sack->custom_prompt);
	ft_free_array(sack->envp);
	if (sack->cmd_tree)
		ft_free_cmdtree(sack->cmd_tree);
	exit(exit_code);
}

void	ft_free_array(char **array)
{
	int	pos;

	pos = 0;
	if (!array)
		return ;
	while (array[pos])
		free(array[pos++]);
	free(array);
}

void	free_cmd_tok(void *tok)
{
	t_cmdtkn	*tofree;

	tofree = tok;
	if (tofree)
	{
		if (tofree->str)
			free(tofree->str);
		free(tofree);
	}
}

char	*get_cwd_str(void)
{
	void	*nullable;
	char	cwd[256];

	nullable = getcwd(cwd, sizeof(cwd));
	if (!nullable)
	{
		perror("µShell: update cwd error");
		return (NULL);
	}
	return (ft_strdup(cwd));
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r')
		return (1);
	return (0);
}
