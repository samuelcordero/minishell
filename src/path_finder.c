/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:52:55 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 22:53:07 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_rel_path(char *path)
{
	if (!ft_strncmp(path, "./", 2) || !ft_strncmp(path, "../", 3)
		|| !ft_strncmp(path, "/", 1))
		return (1);
	return (0);
}

static int	ft_execbultin(t_cmd_node *node, t_mshell_sack *sack)
{
	if (!ft_strncmp(node->args[0], "cd", 3))
		ft_change_dir(node, sack);
	else if (!ft_strncmp(node->args[0], "echo", 5))
		ft_echo(node, sack->envp);
	else if (!ft_strncmp(node->args[0], "exit", 5))
		ft_msh_exit(node, sack->envp);
	else if (!ft_strncmp(node->args[0], "pwd", 4))
		ft_print_working_dir(node, sack->envp);
	else if (!ft_strncmp(node->args[0], "unset", 6))
		ft_unset(node, sack);
	else if (!ft_strncmp(node->args[0], "export", 7))
		ft_export(node, sack);
	return (node->is_builtin);
}

char	*extract_exec_path(t_mshell_sack *sack, t_cmd_node *node)
{
	int		pos;
	char	**split_path;
	char	*tmp;
	char	*path_plus_exec;

	if (!node->args[0])
		return (NULL);
	if (!(access(node->args[0], F_OK)) && ft_is_rel_path(node->args[0]))
		return (node->args[0]);
	if (ft_is_rel_path(node->args[0]))
		return (NULL);
	if (ft_execbultin(node, sack))
		return (NULL);
	if (!sack->envp || !*sack->envp)
		return (NULL);
	split_path = ft_split(ft_get_from_env(sack->envp, "PATH"), ':');
	pos = 0;
	while (split_path[pos])
	{
		tmp = ft_strjoin(split_path[pos++], "/");
		path_plus_exec = ft_strjoin(tmp, node->args[0]);
		free(tmp);
		if (!(access(path_plus_exec, F_OK)))
			return (ft_free_array(split_path), path_plus_exec);
		free(path_plus_exec);
	}
	return (ft_free_array(split_path), NULL);
}
