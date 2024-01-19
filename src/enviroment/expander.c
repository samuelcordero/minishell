/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:34:15 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/18 12:05:51 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *str, int needle_tip)
{
	size_t	len;

	if (str[needle_tip] == '$')
		return (ft_strdup("$"));
	if (str[needle_tip] == '?')
		return (ft_strdup("?"));
	if (ft_isspace(str[needle_tip]) || !str[needle_tip]
		|| str[needle_tip] == '"')
		return (ft_strdup(""));
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

static char	*expand_str(char *str, int *i, char **envp)
{
	char	*needle;
	char	*expanded;
	char	*extract;
	char	*tmp;

	tmp = get_key(str, (*i) + 1);
	needle = ft_strtrim(tmp, " \n\t\r\v");
	free(tmp);
	if (!ft_strncmp(needle, "", 1))
		return (free(needle), ++(*i), str);
	expanded = ft_substr(str, 0, (size_t)(*i));
	tmp = expanded;
	if (!ft_strncmp(needle, "$", 2))
		expanded = ft_strjoin(tmp, "no PID, sorry :C");
	else
		expanded = ft_strjoin(tmp, ft_get_from_env(envp, needle, NULL));
	free(tmp);
	tmp = expanded;
	extract = ft_substr(str, (*i) + 1 + ft_strlen(needle), SIZE_T_MAX);
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
			expanded = expand_str(expanded, &i, envp);
		else if (expanded[i])
			++i;
		if (!expanded)
			break ;
	}
	return (expanded);
}

/*
	incorporar máquina de estados 
	S0: buscar chars
		si * wildcard state
		si " double quote state
		si ' single quote state
		si $ env state
	wildcard state:
		reconoce regex, busca files que hacen match, 
		vuelve a S0 cuando avanza todo el wildcard
	double quote state: ok
		mueve el string para quitar las comillas
		no vuelve a S0 hasta que encuentre una doble comilla
		si encuentra env, pasa a estado env
	single quote state: ok
		mueve el string para quitar las comillas
		no vuelve a S0 hasta que encuentre una comilla simple
	env state:
		reconoce variable tipo $NAME
		reemplaza el valor de la variable en el str
		vuelve a s0 cuando se avanza la longitud del valor $NAME
		si antes se encuentra un wildcard, se pasa a s0 si no venimos del estado de comillas
*/
int	ft_expand_list(t_list *list, t_mshell_sack *sack)
{
	while (list)
	{
		if (((t_cmdtoken *)list->content)->type == ARG)
			expand_list(list, sack);
		else if (((t_cmdtoken *)list->content)->type == E_EXP_ARG)
			expand_list(list, sack);
		else
			list = list->next;
	}
	return (0);
}

/* int	ft_expand_list(t_list *list, t_mshell_sack *sack)
{
	t_cmdtoken	*content;

	while (list)
	{
		content = list->content;
		if (ft_strchr(content->str, '$') && content->type != EXP_ARG)
			ft_expand_env_list(list, sack);
		if (ft_strchr(content->str, '*'))
			ft_expand_wcard_list(list, sack);
		list = list->next;
	}
	return (0);
} */
