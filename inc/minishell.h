/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/10 13:59:39 by sacorder         ###   ########.fr       */
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
# include <fcntl.h>

typedef struct s_cmdtoken
{
	char	*str;
	int		type;
}	t_cmdtoken;

typedef struct s_cmd
{
	char			**args;
	int				pipe_in;
	int				redirect_type;
	char			*file_name;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_cmdtree
{
	struct s_cmdtree	*left;
	struct s_cmdtree	*right;
	t_cmd				*cmd_list;
	char				type;
}	t_cmdtree;

typedef struct s_mshell_sack
{
	t_list		*cmd_tokens;
	t_cmdtree	*cmd_tree;
	char		*line;
	char		**envp;
}	t_mshell_sack;

//command_tree

int		ft_parse_tree(t_cmdtree **tree, t_list *tokenlist);

//init

void	init(t_mshell_sack *sack, char **envp);

//lexer

t_list	*lexer(char	*str);

//lexer_utils

void	state_quote_delimiter(char *str, int *i, char delim);

//expander

void	expand(t_cmdtoken *token, char **envp);

//redirect_utils

void	ft_mid_redirect(char *cmd, char **envp);
int		ft_redirect_in(char *cmd, char **envp, char *if_path, int chained);
int		ft_redirect_out(char *cmd, char **envp, char *of_path, int flags, int chained);

//redirect_utils2

void	ft_perror_exit(char *errmsg, int exitnb);
int		ft_open(char *path, int flags, int perms);
void	ft_close(int fd);
void	ft_dup2(int oldfd, int newfd);
void	ft_free_array(char **array);
char	*extract_exec_path(char **envp, char *name);

//debug only

void	print_tokens(t_list *tokens);

#endif
