/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:34:15 by sacorder          #+#    #+#             */
/*   Updated: 2023/11/20 12:50:20 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *str, int needle_tip)
{
	size_t	len;

	if (str[needle_tip] == '$')
		return (ft_strdup("$"));
	len = ft_strchr(&str[needle_tip], ' ') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], '"') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], '"') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], '|') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], '|') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], ';') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], ';') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], '&') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], '&') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], '$') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], '$') - &str[needle_tip];
	if (len == 0)
		len = SIZE_T_MAX;
	return (ft_substr(str, needle_tip, len));
}

static char	*expand_str(char *str, int i, char **envp)
{
	char	*needle;
	char	*expanded;
	char	*extract;
	char	*tmp;

	needle = get_key(str, i + 1);
	expanded = ft_substr(str, 0, (size_t) i);
	tmp = expanded;
	if (str[i + 1] == '$')
		expanded = ft_strjoin(tmp, "no PID, sorry :C");
	else
		expanded = ft_strjoin(tmp, ft_get_from_env(envp, needle));
	free(tmp);
	tmp = expanded;
	extract = ft_substr(str, i + 1 + ft_strlen(needle), SIZE_T_MAX);
	expanded = ft_strjoin(expanded, extract);
	free(tmp);
	free(extract);
	free(needle);
	free(str);
	return (expanded);
}

/*
	Expands the provided char *line with variables from envp
	E.g.: "Hello $name" expand to "Hello Juan" if name
	is set as Juan inside the enviroment
*/
char	*ft_expand(char *line, char **envp)
{
	int		i;
	int		is_expandable;
	char	*expanded;

	i = -1;
	expanded = ft_strdup(line);
	is_expandable = 1;
	while (expanded[++i])
	{
		if (expanded[i] == '\'')
			is_expandable = !is_expandable;
		if (expanded[i] == '$' && is_expandable)
			expanded = expand_str(expanded, i, envp);
	}
	return (expanded);
}
