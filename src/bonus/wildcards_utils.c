/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:15:01 by guortun-          #+#    #+#             */
/*   Updated: 2024/01/12 15:15:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_char_unop(char token, int index)
{
	if (token != '\0')
		return (index + 1);
	return (index);
}

int	strlen_and_free(char **tmp, char **tmp2, char **file)
{
	int	i;

	*tmp = *tmp2;
	if (*file)
	{
		i = (1 + ft_strlen(*file));
		free(*file);
		return (i);
	}
	else
		return (-1);
}

void	get_files_init(DIR **dir_ptr, struct dirent **directory)
{
	*dir_ptr = opendir(".");
	*directory = readdir(*dir_ptr);
}

int	regex_iterator(char *regex, char *f_name, int *j, int i)
{
	while (regex[*j] == '*')
		++*j;
	if (f_name[i] || (regex[*j]))
		return (0);
	return (1);
}
