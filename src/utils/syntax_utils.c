/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:58:58 by guortun-          #+#    #+#             */
/*   Updated: 2024/01/11 22:01:21 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_is_exec;

int	ft_check_syntax_heredoc(t_mshell_sack *sack)
{
	if (ft_check_quotes(sack->cmd_tree->cmd_str))
		return (ft_add_to_env(sack, "?=2"), 0);
	if (ft_check_brackets(sack->cmd_tree->cmd_str))
		return (ft_add_to_env(sack, "?=2"), 0);
	if (ft_check_fredirs(sack->cmd_tree->cmd_str))
		return (ft_add_to_env(sack, "?=2"), 0);
	g_is_exec = 2;
	if (ft_create_heredocs(&sack->cmd_tree->cmd_str))
		return (g_is_exec = 0, ft_add_to_env(sack, "?=2"), 0);
	if (!g_is_exec)
		return (ft_add_to_env(sack, "?=130"), 0);
	g_is_exec = 0;
	return (1);
}
