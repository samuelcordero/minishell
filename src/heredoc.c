/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:50:36 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/21 16:48:21 by sacorder         ###   ########.fr       */
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

static char	*get_delim_and_substitute(char **str, int *i, char *new_name)
{
	int		j;
	char	*delim;
	char	*tmp;
	char	*tmp2;

	while (ft_isspace((*str)[(*i)]))
		*i += 1;
	j = *i;
	while ((*str)[j] && !ft_isspace((*str)[j]) && !ft_isreserved((*str)[j])
		&& (*str)[j] != '(' && (*str)[j] != ')')
		++j;
	if (j == *i)
		return (NULL);
	delim = ft_substr(*str, *i, j - *i);
	tmp = ft_substr(*str, 0, *i);
	tmp2 = ft_strjoin(tmp, new_name);
	free(tmp);
	tmp = ft_strjoin(tmp2, " ");
	free(tmp2);
	tmp2 = ft_substr(*str, j, SIZE_T_MAX);
	free(*str);
	*str = ft_strjoin(tmp, tmp2);
	free(tmp);
	free(tmp2);
	return (*i += ft_strlen(new_name), delim);
}

static int	create_temp_heredoc(char **str, int *i, char **delim)
{
	int		fd;
	char	*tmp_name;

	tmp_name = get_tmp_filename();
	if (!tmp_name)
		return (ft_putendl_fd("No heredoc tmp file available!", 2),
			exit(1), -1);
	fd = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(tmp_name), free(tmp_name), exit(1), -1);
	*i += 2;
	*delim = get_delim_and_substitute(str, i, tmp_name);
	free(tmp_name);
	if (!*delim)
	{
		ft_close(fd);
		return (-1);
	}
	return (fd);
}

/*
	Creates a tmp heredoc, then fills it with user input.
	Line is expanded with envp variables as in bash
	First finds de delimiter, then creates a tmp file
	Delimiter is changed to absolute path of tmp file
*/
int	ft_heredoc(char **str, int *i, char **envp)
{
	int		fd;
	char	*delim;
	char	*line;
	char	*prompt;
	char	*exp;

	fd = create_temp_heredoc(str, i, &delim);
	if (fd == -1)
		return (1);
	line = ft_strjoin("here_doc (", delim);
	prompt = ft_strjoin(line, ") > ");
	free(line);
	line = readline(prompt);
	while (line && ft_strncmp(line, delim,
			ft_strlen(delim) + 1))
	{
		exp = ft_expand(line, envp);
		ft_putendl_fd(exp, fd);
		free(line);
		free(exp);
		line = readline(prompt);
	}
	return (free(delim), free(line), free(prompt), ft_close(fd), 0);
}
