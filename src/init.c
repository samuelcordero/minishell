/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 20:49:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 22:43:09 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

static void	ft_sig_handler(int signum)
{
	if (signum == SIGINT && !g_is_exec)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_redisplay();
	}
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
	tmp = ft_strjoin(tmp2, " $> ");
	free(tmp2);
	return (tmp);
}

static int	ft_init_envp(t_mshell_sack *sack, char **envp)
{
	int	ctr;

	ctr = -1;
	g_is_exec = 0;
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

static void	ft_fill_envp(t_mshell_sack *sack)
{
	char	*tmp;
	char	*tmp2;
	int		shllvl;

	shllvl = 1 + ft_atoi(ft_get_from_env(sack->envp, "SHLVL"));
	tmp = ft_itoa(shllvl);
	tmp2 = ft_strjoin("SHLVL=", tmp);
	ft_add_to_env(sack, tmp2);
	free(tmp);
	free(tmp2);
	if (!ft_strncmp(ft_get_from_env(sack->envp, "PATH"), "", 1))
		ft_add_to_env(sack, "PATH=/.local/bin:/bin:/usr/local/bin:/usr/bin");
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
	ft_fill_envp(sack);
	sack->cmd_tree = NULL;
	sack->custom_prompt = ft_custom_prompt(sack->envp);
	signal(SIGINT, ft_sig_handler);
	signal(SIGQUIT, ft_sig_handler);
}
