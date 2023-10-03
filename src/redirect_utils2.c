/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:21:53 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/02 16:40:13 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_perror_exit(char *errmsg, int exitnb)
{
	if (errmsg)
		perror(errmsg);
	exit(exitnb);
}

int	ft_open(char *path, int flags, int perms)
{
	int	fd;

	fd = open(path, flags, perms);
	if (fd == -1)
		perror(path);
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

char	*extract_exec_path(char **envp, char *name)
{
	int		pos;
	char	**split_path;
	char	*tmp;
	char	*path_plus_exec;

	if (!name)
		return (NULL);
	if (!(access(name, F_OK)))
		return (name);
	if (!envp || !*envp)
		return (NULL);
	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	split_path = ft_split(*envp + 5, ':');
	pos = 0;
	while (split_path[pos])
	{
		tmp = ft_strjoin(split_path[pos++], "/");
		path_plus_exec = ft_strjoin(tmp, name);
		free(tmp);
		if (!(access(path_plus_exec, F_OK)))
			return (ft_free_array(split_path), path_plus_exec);
		free(path_plus_exec);
	}
	return (ft_free_array(split_path), NULL);
}
