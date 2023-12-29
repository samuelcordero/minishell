/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:53:44 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/29 13:36:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_valid_key(char	*key_val)
{
	int		i;
	char	has_alpha;

	i = -1;
	has_alpha = 0;
	while (key_val[++i] && key_val[i] != '=' && !ft_isspace(key_val[i]))
	{
		if (ft_isalpha(key_val[i]))
			has_alpha = 1;
		else if (!ft_isalnum(key_val[i]))
		{
			has_alpha = 0;
			break ;
		}
	}
	if (i != 0 && has_alpha)
		return (1);
	ft_putstr_fd("minishell: export: not a valid id: '", STDERR_FILENO);
	ft_putstr_fd(key_val, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (0);
}

static int	ft_print_env(char **envp)
{
	int		i;
	int		j;
	char	flag;

	i = -1;
	while (envp[++i])
	{
		j = -1;
		flag = 0;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		while (envp[i][++j])
		{
			ft_putchar_fd(envp[i][j], STDOUT_FILENO);
			if (envp[i][j] == '=')
			{
				flag = 1;
				ft_putchar_fd('"', STDOUT_FILENO);
			}
		}
		if (flag)
			ft_putchar_fd('"', STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}

int	ft_export(t_cmd_node *node, t_mshell_sack *sack)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	node->is_builtin = 1;
	if (!node->args[1])
		return (ft_print_env(sack->envp));
	while (node->args[++i])
	{
		if (ft_is_valid_key(node->args[i]))
			ft_add_to_env(sack, node->args[i]);
		else
			ret = 1;
	}
	return (ret);
}
