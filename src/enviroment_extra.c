/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:55:51 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/25 00:34:55 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_remove_env(t_mshell_sack *sack, char *key)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (1);
	i = 0;
	while (sack->envp && sack->envp[++i])
	{
		if (!ft_strncmp(sack->envp[i], tmp, ft_strlen(tmp)))
		{
			free(sack->envp[i]);
			sack->env_elems--;
			break ;
		}
	}
	while (sack->envp && sack->envp[i])
	{
		sack->envp[i] = sack->envp[i + 1];
		++i;
	}
	return (0);
}
