/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 20:49:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/01 21:03:48 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_sigint_handler(int signum)
{
	(void) signum;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init(void)
{
	signal(SIGINT, ft_sigint_handler);
}
