/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:21:53 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/23 16:31:31 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_perror_exit(char *errmsg, int exitnb)
{
	if (errmsg)
		perror(errmsg);
	exit(exitnb);
}

int	ft_open(t_redir_tok *f_tok)
{
	int	fd;
	int	flags;

	flags = 0;
	if (f_tok->redir_type == INFILE_MASK || f_tok->redir_type == HEREDOC_MASK)
		flags = O_RDONLY;
	else if (f_tok->redir_type == OUTFILE_MASK)
		flags = O_RDWR | O_CREAT | O_TRUNC;
	else if (f_tok->redir_type == CONCATOUT_MASK)
		flags = O_RDWR | O_CREAT | O_APPEND;
	fd = open(f_tok->file_name, flags, 0644);
	if (fd == -1)
		perror(f_tok->file_name);
	return (fd);
}

void	ft_close(int fd)
{
	int	status;

	if (fd < 0)
		return ;
	status = close(fd);
	if (status == -1)
		perror("close");
}

void	ft_dup2(int oldfd, int newfd)
{
	int	status;

	status = dup2(oldfd, newfd);
	if (status == -1)
		perror("dup2");
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
static int	ft_is_rel_path(char *path)
{
	if (!ft_strncmp(path, "./", 2) || !ft_strncmp(path, "../", 3))
		return (1);
	return (0);
}

static int	ft_execbultin(t_cmd_node *node, char **envp)
{
	if (!ft_strncmp(node->args[0], "cd", 3))
		ft_change_dir(node, envp);
	else if (!ft_strncmp(node->args[0], "echo", 5))
		ft_echo(node, envp);
	else if (!ft_strncmp(node->args[0], "exit", 5))
		ft_msh_exit(node, envp);
	else if (!ft_strncmp(node->args[0], "pwd", 4))
		ft_print_working_dir(node, envp);
	/*
	else if (!ft_strncmp(node->args[0], "unset", 6))
		ft_unset(node, envp);
	else if (!ft_strncmp(node->args[0], "export", 7))
		ft_export(node, envp); */
	return (node->is_builtin);
}

char	*extract_exec_path(char **envp, t_cmd_node *node)
{
	int		pos;
	char	**split_path;
	char	*tmp;
	char	*path_plus_exec;

	if (!node->args[0])
		return (NULL);
	if (!(access(node->args[0], F_OK)))
		return (node->args[0]);
	if (ft_is_rel_path(node->args[0]))
		return (NULL);
	if (!envp || !*envp)
		return (NULL);
	if (ft_execbultin(node, envp))
		return (NULL);
	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	split_path = ft_split(*envp + 5, ':');
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
