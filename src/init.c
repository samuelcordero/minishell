/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 20:49:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 16:14:43 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_sigint_handler(int signum)
{
	(void) signum;
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_redisplay();
}

static char	*ft_custom_prompt(char **envp)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_get_from_env(envp, "USER");
	if (tmp)
		tmp2 = ft_strjoin(PROMPT_BASE, tmp);
	else
		tmp2 = ft_strjoin(PROMPT_BASE, "");
	if (tmp)
		free(tmp);
	tmp = ft_strjoin(tmp2, " $> ");
	free(tmp2);
	return (tmp);
}

static void	ft_nothing(int signum)
{
	(void)signum;
}

static int	ft_init_envp(t_mshell_sack *sack, char **envp)
{
	int	ctr;

	ctr = -1;
	while (envp && envp[sack->env_elems])
		sack->env_elems++;
	sack->env_size = sack->env_elems * 2;
	if (sack->env_size > INT_MAX)
		sack->env_size = INT_MAX;
	else if (sack->env_size == 0)
		sack->env_size = 32;
	sack->envp = ft_calloc(sack->env_size, sizeof(char *));
	if (!sack->envp)
		return (1);
	while (++ctr < sack->env_elems)
		sack->envp[ctr] = ft_strdup(envp[ctr]);
	return (0);
}

void	init(t_mshell_sack *sack, char **envp, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_bzero(sack, sizeof(t_mshell_sack));
	if (ft_init_envp(sack, envp))
	{
		ft_putendl_fd("minishell: error allocating envp space", STDERR_FILENO);
		exit(1);
	}	
	sack->cmd_tree = NULL;
	sack->custom_prompt = ft_custom_prompt(sack->envp);
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_nothing);
}
