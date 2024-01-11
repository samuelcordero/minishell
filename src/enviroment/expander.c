/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:34:15 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/11 14:47:05 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *str, int needle_tip)
{
	size_t	len;

	if (str[needle_tip] == '$' || ft_isspace(str[needle_tip])
		|| !str[needle_tip] || str[needle_tip] == '"')
		return (ft_strdup("$"));
	if (str[needle_tip] == '?')
		return (ft_strdup("?"));
	len = ft_strchr(&str[needle_tip], ' ') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], '"') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], '"') - &str[needle_tip];
	if (len > (size_t)(ft_strchr(&str[needle_tip], '\'') - &str[needle_tip]))
		len = ft_strchr(&str[needle_tip], '\'') - &str[needle_tip];
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

	tmp = get_key(str, i + 1);
	needle = ft_strtrim(tmp, " \n\t\r\v");
	free(tmp);
	if (!ft_strncmp(needle, "$", 2))
		return (free(needle), str);
	expanded = ft_substr(str, 0, (size_t) i);
	tmp = expanded;
	if (str[i + 1] == '$')
		expanded = ft_strjoin(tmp, "no PID, sorry :C");
	else
		expanded = ft_strjoin(tmp, ft_get_from_env(envp, needle, NULL));
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
	Doesnt expand variables between single quotes except expand_all set to 1
*/
char	*ft_expand(char *line, char **envp, char expand_all)
{
	int		i;
	char	in_quotes;
	char	*expanded;

	i = 0;
	expanded = ft_strdup(line);
	in_quotes = 0;
	while (expanded[i])
	{
		if (expanded[i] == '\"')
			in_quotes = !in_quotes;
		if (expanded[i] == '\'' && !in_quotes && !expand_all)
			state_quote_delimiter(expanded, &i, '\'');
		if (expanded[i] == '$')
			expanded = expand_str(expanded, i, envp);
		else
			++i;
		if (!expanded)
			break ;
	}
	return (expanded);
}

int	ft_expand_tokens(t_cmd_node *cmd_list, t_mshell_sack *sack)
{
	while (cmd_list)
	{
		cmd_list->args = ft_expand_arg_arr(cmd_list->args, sack->envp);
		if (!cmd_list->args)
			return (ft_memory_err_exit(sack), 1);
		cmd_list = cmd_list->next;
	}
	return (0);
}
