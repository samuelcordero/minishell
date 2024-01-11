/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:50:36 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/11 22:55:05 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

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
	{
		if ((*str)[j] == '\'' || (*str)[j] == '"')
			state_quote_delimiter(*str, &j, (*str)[j]);
		else
			++j;
	}
	if (j == *i)
		return (NULL);
	delim = ft_substr(*str, *i, j - *i);
	tmp = ft_substr(*str, 0, *i);
	tmp2 = ft_strjoin(tmp, new_name);
	delim_subs(*str, j, tmp, tmp2);
	return (*i += ft_strlen(new_name), delim);
}

static int	create_temp_heredoc(char **str, int *i, char **delim, char **f_name)
{
	int		fd;

	*f_name = get_tmp_filename();
	if (!*f_name)
		return (ft_putendl_fd("No heredoc tmp file available!", 2),
			exit(1), -1);
	fd = open(*f_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(*f_name), free(*f_name), exit(1), -1);
	*i += 2;
	*delim = get_delim_and_substitute(str, i, *f_name);
	if (!*delim)
	{
		ft_close(fd);
		return (-1);
	}
	return (fd);
}

int	ft_expand_heredoc(int o_fd, t_redir_tok *tok, char **envp)
{
	int		fd;
	char	*line;
	char	*exp;

	unlink(tok->file_name);
	free(tok->file_name);
	tok->file_name = get_tmp_filename();
	if (!tok->file_name)
		return (ft_putendl_fd("No heredoc tmp file available!", 2),
			exit(1), -1);
	fd = open(tok->file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(tok->file_name), free(tok->file_name), exit(1), -1);
	line = get_next_line(o_fd);
	while (line)
	{
		exp = ft_expand(line, envp, 1);
		ft_putstr_fd(exp, fd);
		free(line);
		free(exp);
		line = get_next_line(o_fd);
	}
	return (fd);
}

/*
	Creates a tmp heredoc, then fills it with user input.
	Line is expanded with envp variables as in bash
	First finds de delimiter, then creates a tmp file
	Delimiter is changed to absolute path of tmp file
*/
int	ft_heredoc(char **str, int *i, char **f_name)
{
	int		fd;
	char	*delim;
	char	*line;
	char	*prompt;
	int		pid;

	fd = create_temp_heredoc(str, i, &delim, f_name);
	ft_str_unquote(&delim);
	if (fd == -1 || !g_is_exec)
		return (free(delim), 1);
	pid = fork();
	if (pid < 0)
		return (free(delim), 1);
	if (pid)
		return (free(delim), 0);
	signal(SIGINT, SIG_DFL);
	line = ft_strjoin("here_doc (", delim);
	prompt = ft_strjoin(line, ") > ");
	free(line);
	line = readline(prompt);
	heredoc_management(line, prompt, delim, &fd);
	return (free(delim), free(line), free(prompt),
		ft_close(fd), free(*f_name), exit(0), 0);
}
