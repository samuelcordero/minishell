/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:43:30 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/12 15:53:58 by sacorder         ###   ########.fr       */
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
			while (regex[j] == '*')
				++j;
			while (f_name[i] && f_name[i] != regex[j])
				++i;
		}
		if (f_name[i] != regex[j])
			return (0);
		j = if_char_unop(f_name[i], j);
		i = if_char_unop(f_name[i], i);
	}
	if (!regex_iterator(regex, f_name, &j, i))
		return (0);
	return (1);
}

t_list	*ft_get_files(char *regex)
{
	DIR				*dir_ptr;
	struct dirent	*directory;
	char			*tmp;
	t_list			*file_list;
	t_list			*iters[2];

	tmp = ft_strdup(regex);
	ft_str_unquote(&tmp);
	get_files_init(&dir_ptr, &directory);
	file_list = NULL;
	if (!directory)
		return (free(tmp), NULL);	
	file_list = ft_calloc(1, sizeof(t_list));
	iters[0] = file_list;
	iters[1] = iters[0];
	while (directory)
	{
		if (ft_match(directory->d_name, regex))
		{
			iters[0]->content = ft_calloc(1, sizeof(t_cmdtoken));
			((t_cmdtoken *)iters[0]->content)->str = ft_strdup(directory->d_name);
			((t_cmdtoken *)iters[0]->content)->type = 3;
			iters[0]->next = ft_calloc(1, sizeof(t_list));
			iters[1] = iters[0];
			iters[0] = iters[0]->next;
		}
		directory = readdir(dir_ptr);
	}
	free(tmp);
	iters[1]->next = NULL;
	if (iters[0] == iters[1])
		file_list = NULL;
	ft_lstclear(&iters[0], free_cmd_tok);
	return (closedir(dir_ptr), file_list);
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
