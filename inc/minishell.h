/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/22 22:31:58 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "minishell_defines.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

//builtins

int		ft_msh_exit(t_cmd_node *node, char **envp);
int		ft_change_dir(t_cmd_node *node, char **envp);
int		ft_print_working_dir(t_cmd_node *node, char **envp);
int		ft_echo(t_cmd_node *node, char **envp);

//command_tree

int		ft_parse_tree(t_cmdtree **tree, t_list **tokenlist);

//init

void	init(t_mshell_sack *sack, char **envp, int argc, char **argv);

//lexer

t_list	*lexer(char	*str);

//lexer_utils

void	state_quote_delimiter(char *str, int *i, char delim);

//execute

int		execute(t_cmdtree *t_node, char **envp);

//expander

void	expand(t_cmdtoken *token, char **envp);

//redirect_utils

void	ft_perror_exit(char *errmsg, int exitnb);
int		ft_open(t_redir_tok *f_tok);
void	ft_close(int fd);
void	ft_dup2(int oldfd, int newfd);
void	ft_free_array(char **array);
char	*extract_exec_path(char **envp, t_cmd_node *node);

//debug only

void	print_tokens(t_list *tokens);

#endif
