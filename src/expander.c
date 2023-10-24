/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:34:15 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/25 00:39:01 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_needle(char *str, int needle_tip)
{
	size_t	len;

	len = ft_strchr(&str[needle_tip], ' ') - &str[needle_tip];
	if (len == 0)
		len = MAX_SIZE;
	return (ft_substr(str, needle_tip, len));
}

static char	*expand_str(char *str, int i, char **envp)
{
	char	*needle;
	char	*expanded;
	char	*extract;
	char	*tmp;

	needle = get_needle(str, i + 1);
	ft_printf("Needle: %s\n", needle);
	expanded = ft_substr(str, 0, (size_t) i);
	tmp = expanded;
	expanded = ft_strjoin(tmp, ft_get_from_env(envp, needle));
	free(tmp);
	tmp = expanded;
	extract = ft_substr(str, i + 1 + ft_strlen(needle), MAX_SIZE);
	expanded = ft_strjoin(expanded, extract);
	free(tmp);
	free(extract);
	free(needle);
	free(str);
	return (expanded);
}

void	expand(t_cmdtoken *token, char **envp)
{
	char	*str;
	int		i;

	i = -1;
	str = token->str;
	while (str[++i])
	{
		if (str[i] == '$')
			str = expand_str(str, i, envp);
	}
	token->str = str;
}
