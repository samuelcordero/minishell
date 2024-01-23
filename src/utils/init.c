/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 20:49:10 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/23 13:08:37 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

static void	ft_sig_handler(int signum)
{
	if (signum == SIGINT && (!g_is_exec || g_is_exec == 3))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		ft_putendl_fd("\n^C", STDERR_FILENO);
		rl_redisplay();
		g_is_exec = 3;
	}
	else if (signum == SIGQUIT && g_is_exec == 1)
	{
		ft_putstr_fd("\nQuit (core dumped)", STDERR_FILENO);
		g_is_exec = 0;
	}
	else if (signum == SIGINT && g_is_exec == 1)
		g_is_exec = 0;
	else if (signum == SIGINT && g_is_exec == 2)
	{
		g_is_exec = 0;
		ft_putendl_fd("^C", STDERR_FILENO);
	}
}

static char	*ft_custom_prompt(char **envp)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_get_from_env(envp, "USER", NULL);
	if (tmp)
		tmp2 = ft_strjoin(PROMPT_BASE, tmp);
	else
		tmp2 = ft_strjoin(PROMPT_BASE, "");
	tmp = ft_strjoin(tmp2, "\001\x1b[0m\002 $> ");
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

	shllvl = 1 + ft_atoi(ft_get_from_env(sack->envp, "SHLVL", NULL));
	if (shllvl >= 1000)
		tmp_management(&tmp, &shllvl);
	tmp = ft_itoa(shllvl);
	tmp2 = ft_strjoin("SHLVL=", tmp);
	ft_add_to_env(sack, tmp2);
	free(tmp);
	free(tmp2);
	tmp = get_cwd_str();
	if (tmp)
	{
		tmp2 = ft_strjoin("PWD=", tmp);
		ft_add_to_env(sack, tmp2);
		free(tmp);
		free(tmp2);
	}
	ft_add_to_env(sack, "?=0");
}

/*
	Initializes main struct,
	custom envp (allocation and copy of given envp,
	plus setting of varibles such as SHLVL),
	creates a custom prompt, sets signal handling 
	and creates persistent history file if posible
*/
void	init(t_mshell_sack *sack, char **envp, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ft_bzero(sack, sizeof(t_mshell_sack));
	if (ft_init_envp(sack, envp))
	{
		ft_putendl_fd("minishell: error allocating envp space", STDERR_FILENO);
		ft_memory_err_exit(sack);
	}
	ft_fill_envp(sack);
	sack->cmd_tree = NULL;
	sack->custom_prompt = ft_custom_prompt(sack->envp);
	signal(SIGINT, ft_sig_handler);
	signal(SIGQUIT, ft_sig_handler);
	ft_set_signal_print(0);
	ft_init_history_file(sack);
	ft_fill_history(sack);
}
