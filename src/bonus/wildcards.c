/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:43:30 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/24 14:05:13 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_match(char *f_name, char *regex)
{
	int	cont[2];
	
	ft_memset(cont, 0, sizeof(int) * 2);
	if (!ft_strncmp(".", f_name, 2) || !ft_strncmp("..", f_name, 3)
		|| (f_name[0] == '.' && regex[0] != '.'))
		return (0);
	while (f_name[cont[0]] && regex[cont[1]])
	{
		if (regex[cont[1]] == '*')
		{
			while (regex[cont[1]] == '*')
				++cont[1];
			while (f_name[cont[0]] && f_name[cont[0]] != regex[cont[1]])
				++cont[0];
		}
		if (f_name[cont[0]] != regex[cont[1]])
			return (0);
		cont[1] = if_char_unop(f_name[cont[0]], cont[1]);
		cont[0] = if_char_unop(f_name[cont[0]], cont[0]);
		if (!regex[cont[1]] && f_name[cont[0]])
		{
			while (regex[cont[1]] != '*')
				--cont[1];
		}
	}
	if (!regex_iterator(regex, f_name, &cont[1], cont[0]))
		return (0);
	return (1);
}

char	*ft_get_files(char *regex)
{
	DIR				*dir_ptr;
	struct dirent	*directory;
	char			*file_list;
	char			*tmp;

	get_files_init(&dir_ptr, &directory);
	file_list = ft_strdup("");
	if (!directory || !file_list)
		return (NULL);
	while (directory)
	{
		if (ft_match(directory->d_name, regex))
		{
			tmp = ft_strjoin(file_list, directory->d_name);
			free(file_list);
			file_list = ft_strjoin(tmp, " ");
			free(tmp);
		}
		directory = readdir(dir_ptr);
	}
	if (!file_list[0])
		return (free(file_list), closedir(dir_ptr), NULL);
	tmp = ft_strtrim(file_list, " \n\t\r\v");
	free(file_list);
	return (closedir(dir_ptr), tmp);
}

/* static char	*ft_get_regex(char *regex_start, int *i)
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
		dist = strlen_and_free(&tmp, &tmp2, &files[ctr]);
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
} */
