/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:21:53 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 22:52:24 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (fd < 0)
		return ;
	if (close(fd) == -1)
		perror("close");
}

void	ft_dup2(int oldfd, int newfd)
{
	int	status;

	status = dup2(oldfd, newfd);
	if (status == -1)
		perror("dup2");
}
