/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/07/05 17:20:01 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmdtoken
{
	char	*str;
	int		type;
}		t_cmdtoken;

t_list	*lexer(char	*str);

//debug only

void	print_tokens(t_list *tokens);

#endif