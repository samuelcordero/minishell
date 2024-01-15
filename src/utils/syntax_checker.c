/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:28:35 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/15 14:10:21 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

void	ft_print_syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}

int	ft_check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			state_quote_delimiter(str, &i, str[i]);
			if (!str[i])
			{
				ft_print_syntax_error("unmatching number of quotes");
				return (1);
			}
			else
				++i;
		}
		else
			++i;
	}
	return (0);
}

int	ft_check_brackets(char *str)
{
	int	opening;
	int	closing;
	int	i;

	i = 0;
	opening = 0;
	closing = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			state_quote_delimiter(str, &i, str[i]);
		else if (str[i] == '(')
			++opening;
		else if (str[i] == ')')
			++closing;
		if (str[i])
			++i;
	}
	if (closing != opening)
	{
		ft_print_syntax_error("unmatching number of brackets");
		return (1);
	}
	return (0);
}

int	ft_create_heredocs(char **str)
{
	int		i;
	char	*f_name;

	i = 0;
	f_name = NULL;
	while ((*str)[i] && g_is_exec)
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			state_quote_delimiter(*str, &i, (*str)[i]);
		if (!ft_strncmp("<<", &(*str)[i], 2))
			if (ft_heredoc(str, &i, &f_name))
				return (1);
		waitpid(-1, NULL, 0);
		if (f_name)
		{
			if (!g_is_exec)
				unlink(f_name);
			free(f_name);
			f_name = NULL;
		}
		if ((*str)[i])
			++i;
	}
	return (0);
}

int	ft_check_fredirs(char *str)
{
	int		i;
	int		check;

	i = 0;
	check = 0;
	while (str[i] && !check)
	{
		if (str[i] == '\'' || str[i] == '"')
			state_quote_delimiter(str, &i, str[i]);
		else if (!ft_strncmp("<<", &str[i], 2) || !ft_strncmp("<", &str[i], 1)
			|| !ft_strncmp(">>", &str[i], 2) || !ft_strncmp(">", &str[i], 1))
			check = ft_check_f_name(str, &i);
		else
			++i;
		if (check)
			return (check);
	}
	return (0);
}
