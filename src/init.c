/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 20:49:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/23 21:29:47 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_sigint_handler(int signum)
{
	(void) signum;
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_printf("\n");
	rl_redisplay();
}

static char	*ft_custom_prompt(char **envp)
{
	char	*tmp;
	char	*tmp2;

	//tmp = ft_strjoin(PROMPT_BASE, " - \x1b[31m");
	tmp2 = ft_strjoin(PROMPT_BASE, get_from_env(envp, "USER"));
	//free(tmp);
	tmp = ft_strjoin(tmp2, " $> ");
	free(tmp2);
	return (tmp);
}

static void	ft_nothing(int signum)
{
	(void)signum;
}

void	init(t_mshell_sack *sack, char **envp, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_bzero(sack, sizeof(t_mshell_sack));
	sack->envp = envp;
	sack->cmd_tree = NULL;
	sack->custom_prompt = ft_custom_prompt(envp);
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_nothing);
}
