/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/01 20:58:47 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAX_SIZE 65535
# define PROMPT "Mini\x1b[32mShell\x1b[0m $> "

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>

typedef struct s_cmdtoken
{
	char	*str;
	int		type;
}		t_cmdtoken;

//init

void	init(void);

//lexer

t_list	*lexer(char	*str);

//lexer_utils

void	state_quote_delimiter(char *str, int *i, char delim);

//expander

void	expand(t_cmdtoken *token, char **envp);

//debug only

void	print_tokens(t_list *tokens);

#endif
