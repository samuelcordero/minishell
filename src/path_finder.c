/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:52:55 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/12 23:18:49 by sacorder         ###   ########.fr       */
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

void	ft_execbuiltin(t_cmd_node *node, t_mshell_sack *sack, int parent)
{
	int	e_code;

	e_code = 0;
	if (parent)
	{
		if (ft_file_redirs(node->redirs_lst, STDIN_FILENO, STDOUT_FILENO))
			return ;
	}
	if (!ft_strncmp(node->args[0], "cd", 3))
		e_code = ft_change_dir(node, sack);
	else if (!ft_strncmp(node->args[0], "echo", 5))
		e_code = ft_echo(node);
	else if (!ft_strncmp(node->args[0], "exit", 5))
		e_code = ft_msh_exit(node, sack);
	else if (!ft_strncmp(node->args[0], "pwd", 4))
		e_code = ft_print_working_dir(node);
	else if (!ft_strncmp(node->args[0], "unset", 6))
		e_code = ft_unset(node, sack);
	else if (!ft_strncmp(node->args[0], "export", 7))
		e_code = ft_export(node, sack);
	node->exit_code = e_code;
	if (!parent)
		exit(e_code);
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
		return (ft_strdup(node->args[0]));
	if (ft_isbuiltin(node->args[0]))
		return (NULL);
	if (ft_is_rel_path(node->args[0]) || (!sack->envp || !*sack->envp))
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
