/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:43:30 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/18 14:12:44 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_match(char *f_name, char *regex)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!ft_strncmp(".", f_name, 2) || !ft_strncmp("..", f_name, 3)
		|| (f_name[0] == '.' && regex[0] != '.'))
		return (0);
	while (f_name[i] && regex[j])
	{
		if (regex[j] == '*')
		{
			++j;
			while (f_name[i] && f_name[i] != regex[j])
				++i;
		}
		if (f_name[i] != regex[j])
			return (0);
		if (f_name[i])
		{
			++i;
			++j;
		}
	}
	if (f_name[i] || regex[j])
		return (0);
	return (1);
}

static char	**ft_get_files(char *regex)
{
	DIR				*dir_ptr;
	struct dirent	*directory;
	char			**matches;
	int				ctr;
	char			*tmp;

	dir_ptr = opendir(".");
	directory = readdir(dir_ptr);
	matches  = ft_calloc(1024, sizeof(char *));
	ctr = -1;
	while (directory)
	{
		if (ft_match(directory->d_name, regex))
		{
			tmp = ft_strjoin("'", directory->d_name);
			//if (!tmp) error
			matches[++ctr] = ft_strjoin(tmp, "'");
			free(tmp);
			//if (!matches[ctr]) error
		}
		directory = readdir(dir_ptr);
	}
	closedir(dir_ptr);
	return (matches);
}

static char	*ft_get_regex(char *regex_start, int *i)
{
	int		j;
	char	*regex;
	
	j = 0;
	while (!ft_isspace(regex_start[j]) && regex_start[j] != '\''
		&& regex_start[j] != '\"' && regex_start[j])
		++j;
	regex = ft_substr(regex_start, 0, j);
	if (ft_strchr(regex, '*'))
		return (regex);
	free(regex);
	*i += j - 1;
	return (NULL);
}

static char	*ft_join_files(char *str, char **files, int *i, char *regex)
{
	char	*res;
	char	*tmp;
	char	*tmp2;
	int		ctr;
	int		dist;

	tmp = ft_substr(str, 0, *i);
	ctr = -1;
	dist = 0;
	while (files[++ctr])
	{
		tmp2 = ft_strjoin(tmp, files[ctr]);
		free(tmp);
		dist += 1 + ft_strlen(files[ctr]);
		free(files[ctr]);
		tmp = tmp2;
		if (files[ctr + 1])
		{
			tmp2 = ft_strjoin(tmp, " ");
			free(tmp);
			tmp = tmp2;
		}
	}
	tmp2 = ft_substr(str, *i + ft_strlen(regex), SIZE_T_MAX);
	res = ft_strjoin(tmp, tmp2);
	*i += dist - 2;
	return (free(str), free(tmp), free(tmp2), res);
}

/*
	1. find * regex
	2. get file table from regex
	3. join substring until regex with file table,
		then join substring past regex
	this is done in a loop until the end of the string
*/
char	*ft_expand_wildcards(char *str)
{
	char	**f_table;
	char	*regex;
	int		i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\"')
			state_quote_delimiter(str, &i, '\"');
		if (str[i] == '\'')
			state_quote_delimiter(str, &i, '\'');
		while (ft_isspace(str[i]))
			++i;
		regex = ft_get_regex(&str[i], &i);
		if (regex)
		{
			f_table = ft_get_files(regex);
			if (f_table[0])
				str = ft_join_files(str, f_table, &i, regex);
			else
				i += ft_strlen(regex) - 1;
			free(regex);
			free(f_table);
		}
	}
	return (str);
}
