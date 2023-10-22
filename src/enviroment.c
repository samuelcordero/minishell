/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 23:48:33 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/23 00:10:32 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_from_env(char **envp, char	*key)
{
	char	*tmp;
	char	*ret;
	int		i;

	tmp = ft_strjoin(key, "=");
	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], tmp, ft_strlen(tmp)))
		{
			ret = envp[i] + ft_strlen(tmp);
			free(tmp);
			return (ret);
		}
	}
	return (NULL);
}