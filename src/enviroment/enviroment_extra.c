/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 22:55:51 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/22 14:53:43 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_remove_env(t_mshell_sack *sack, char *key)
{
	int	i;
	int	j;

	i = -1;
	while (sack->envp && sack->envp[++i])
	{
		j = 0;
		while (sack->envp[i][j] && key[j] && (sack->envp[i][j] == key[j]))
			++j;
		if (!key[j] && (!sack->envp[i][j] || sack->envp[i][j] == '='))
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

void	ft_envp_tidy(t_mshell_sack *sack)
{
	int	i;
	int	j;

	i = -1;
	while (sack->envp && sack->envp[++i])
	{
		if (!ft_strchr(sack->envp[i], '='))
		{
			free(sack->envp[i]);
			j = --i;
			while (sack->envp[++j])
				sack->envp[j] = sack->envp[j + 1];
		}
	}
}
