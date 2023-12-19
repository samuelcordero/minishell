/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:58:46 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/19 15:53:46 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isbuiltin(char *str)
{
	if (!ft_strncmp(str, "cd", 3))
		return (1);
	else if (!ft_strncmp(str, "echo", 5))
		return (1);
	else if (!ft_strncmp(str, "exit", 5))
		return (1);
	else if (!ft_strncmp(str, "pwd", 4))
		return (1);
	else if (!ft_strncmp(str, "unset", 6))
		return (1);
	else if (!ft_strncmp(str, "export", 7))
		return (1);
	return (0);
}

static int	ft_is_valid_key(char	*key_val)
{
	int		i;
	char	has_alpha;

	i = -1;
	has_alpha = 0;
	while (key_val[++i] && key_val[i] != '=' && key_val[i] != '.'
		&& key_val[i] != '-' && !ft_isspace(key_val[i]))
	{
		if (ft_isalpha(key_val[i]))
			has_alpha = 1;
	}
	if (key_val[i] == '=' && i != 0 && has_alpha)
		return (1);
	ft_putstr_fd("minishell: export: not a valid id: '", STDERR_FILENO);
	ft_putstr_fd(key_val, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
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
		return (0); //should also print env
	while (node->args[++i])
	{
		if (ft_is_valid_key(node->args[i]))
			ft_add_to_env(sack, node->args[i]);
		else
			ret = 1;
	}
	return (ret);
}

int	ft_unset(t_cmd_node *node, t_mshell_sack *sack)
{
	int	i;

	i = 0;
	node->is_builtin = 1;
	while (node->args[++i])
		ft_remove_env(sack, node->args[i]);
	return (0);
}
