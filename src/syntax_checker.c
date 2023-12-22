/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:28:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/22 15:01:40 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}

static int	ft_check_quotes(char *str)
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

static int	ft_check_brackets(char *str)
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

static int	ft_create_heredocs(char **str, char **envp)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			state_quote_delimiter(*str, &i, (*str)[i]);
		if (!ft_strncmp("<<", &(*str)[i], 2))
			ft_heredoc(str, &i, envp);
		if ((*str)[i])
			++i;
	}
	return (0);
}

int	ft_check_syntax_heredoc(t_mshell_sack *sack)
{
	if (ft_check_quotes(sack->cmd_tree->cmd_str))
		return (ft_add_to_env(sack, "?=2"), 0);
	if (ft_check_brackets(sack->cmd_tree->cmd_str))
		return (ft_add_to_env(sack, "?=2"), 0);
	if (ft_create_heredocs(&sack->cmd_tree->cmd_str, sack->envp))
		return (ft_add_to_env(sack, "?=2"), 0);
	return (1);
}
