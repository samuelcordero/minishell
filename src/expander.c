/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:34:15 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/18 21:16:23 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *str, int needle_tip)
{
	size_t	len;

	if (str[needle_tip] == '$' || ft_isspace(str[needle_tip]) || !str[needle_tip] || str[needle_tip] == '"')
		return (ft_strdup("$"));
	if (str[needle_tip] == '?')
		return (ft_strdup("?"));
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
	if (!ft_strncmp(needle, "$", 2))
		return (free(needle), str);
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
	Doesnt expand variables between single quotes
*/
char	*ft_expand(char *line, char **envp)
{
	int		i;
	char	in_quotes;
	char	*expanded;

	i = -1;
	expanded = ft_strdup(line);
	in_quotes = 0;
	while (expanded[++i])
	{
		if (expanded[i] == '\"')
			in_quotes = !in_quotes;
		if (expanded[i] == '\'' && !in_quotes)
			state_quote_delimiter(expanded, &i, '\'');
		if (expanded[i] == '$')
			expanded = expand_str(expanded, i, envp);
		if (!expanded || !expanded[i])
			break ;
	}
	return (expanded);
}
