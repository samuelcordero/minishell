/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:31:34 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/02 20:56:39 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_printexit(int exit_code, t_mshell_sack *sack)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	close(sack->history_fd);
	free(sack->custom_prompt);
	ft_free_array(sack->envp);
	if (sack->cmd_tree)
		ft_free_cmdtree(sack->cmd_tree);
	if (sack->cmd_tokens)
		ft_lstclear(&sack->cmd_tokens, free_cmd_tok);
	if (sack->line)
		free(sack->line);
	if (sack->expanded)
		free(sack->expanded);
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

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r')
		return (1);
	return (0);
}
