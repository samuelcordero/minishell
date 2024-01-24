/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 23:48:33 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/24 12:17:57 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_realloc_and_add_envp(t_mshell_sack *sack, char *key_val)
{
	char	**res;
	int		ctr;

	ctr = -1;
	if (sack->env_size + 32 < INT_MAX)
		sack->env_size += 32;
	else
		return (1);
	res = ft_calloc(sack->env_size, sizeof(char *));
	if (!res)
		return (1);
	while (sack->envp[++ctr])
		res[ctr] = sack->envp[ctr];
	res[ctr] = ft_strdup(key_val);
	sack->env_elems++;
	free(sack->envp);
	sack->envp = res;
	return (0);
}

static void	ft_search_and_replace_env(char **envp, char *key_val, char *key)
{
	int		i;

	i = -1;
	while (envp && envp[++i])
	{
		if (!ft_strncmp(envp[i], key, ft_strlen(key)))
		{
			if (envp[i][ft_strlen(key)] == '='
				|| !envp[i][ft_strlen(key)])
			{
				free(envp[i]);
				envp[i] = ft_strdup(key_val);
				break ;
			}
		}
	}
}

static int	ft_add_env_new(t_mshell_sack *sack, char *key_val)
{
	if (sack->env_elems + 1 < sack->env_size)
	{
		sack->envp[sack->env_elems] = ft_strdup(key_val);
		sack->env_elems += 1;
	}
	else
	{
		if (ft_realloc_and_add_envp(sack, key_val))
		{
			ft_putendl_fd("Minishell: couldn't add more envp tuples",
				STDERR_FILENO);
			ft_memory_err_exit(sack);
		}
	}
	return (0);
}

char	*ft_get_from_env(char **envp, char	*key, int *exists)
{
	int		i;

	i = -1;
	if (!ft_strncmp(key, "", 1))
		return ("$");
	if (!ft_strncmp(key, "$", 2))
		return ("no PID, sorry :C");
	while (envp && envp[++i])
	{
		if (!ft_strncmp(envp[i], key, ft_strlen(key)))
		{
			if (envp[i][ft_strlen(key)] == '=')
			{
				if (exists)
					*exists = 1;
				return (envp[i] + ft_strlen(key) + 1);
			}
			if (!envp[i][ft_strlen(key)])
			{
				if (exists)
					*exists = 1;
			}
		}
	}
	return ("");
}

int	ft_add_to_env(t_mshell_sack *sack, char *key_val)
{
	char	*key;
	int		exists;

	exists = 0;
	if (ft_strchr(key_val, '='))
		key = ft_substr(key_val, 0, ft_strchr(key_val, '=') - key_val);
	else
		key = ft_strdup(key_val);
	if ((!ft_strncmp(ft_get_from_env(sack->envp, key, &exists), "", 1)
			&& exists && ft_strchr(key_val, '='))
		|| (exists && ft_strchr(key_val, '=')))
		ft_search_and_replace_env(sack->envp, key_val, key);
	else if (!exists)
	{
		if (ft_add_env_new(sack, key_val))
		{
			free(key);
			return (1);
		}
	}
	free(key);
	return (0);
}
