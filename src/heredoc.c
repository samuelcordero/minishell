/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:50:36 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/24 17:46:07 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_tmp_filename(void)
{
	char	*name;
	char	*tmp;
	int		i;

	i = 0;
	while (++i <= 1000)
	{
		tmp = ft_itoa(i);
		name = ft_strjoin(H_DOC_TMP_BASE, tmp);
		free(tmp);
		if (access(name, F_OK) != 0)
			return (name);
		free(name);
	}
	return (NULL);
}

static int	create_temp_heredoc(t_redir_tok *tok)
{
	int		fd;
	char	*tmp_name;

	tmp_name = get_tmp_filename();
	if (!tmp_name)
		return (ft_putendl_fd("No heredoc tmp file available!", 2), -1);
	fd = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		perror(tmp_name);
	tok->file_name = tmp_name;
	return (fd);
}

int	ft_heredoc(t_redir_tok *tok)
{
	int		fd;
	char	*delim;
	char	*line;
	char	*prompt;

	delim = tok->file_name;
	fd = create_temp_heredoc(tok);
	if (fd < 0)
		return (free(delim), ft_putendl_fd("Heredoc error!", 2), 1);
	line = ft_strjoin("here_doc (", delim);
	prompt = ft_strjoin(line, ") > ");
	free(line);
	line = readline(prompt);
	while (line && ft_strncmp(line, delim,
			ft_strlen(delim) + 1))
	{
		ft_putendl_fd(line, fd);
		free(line);
		line = readline(prompt);
	}
	return (free(delim), free(line), free(prompt), ft_close(fd), 0);
}
