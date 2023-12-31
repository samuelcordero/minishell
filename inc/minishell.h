/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:16:35 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/28 19:14:15 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "minishell_defines.h"
# include <stdio.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

//builtins

int			ft_msh_exit(t_cmd_node *node, t_mshell_sack *sack, char print);
int			ft_change_dir(t_cmd_node *node, t_mshell_sack *sack);
int			ft_print_working_dir(t_cmd_node *node, char **envp);
int			ft_echo(t_cmd_node *node);

//builtins_extra

int			ft_export(t_cmd_node *node, t_mshell_sack *sack);
int			ft_unset(t_cmd_node *node, t_mshell_sack *sack);
int			ft_isbuiltin(char *str);

//command_tree

int			ft_fill_cmdlist(t_list *begin, t_cmdtree *tree_node);
int			ft_parse_tree(t_cmdtree **tree, t_list **tokenlist);

//command_tree_utils

void		*ft_free_cmdtree(t_cmdtree *tree);

//enviroment

char		*ft_get_from_env(char **envp, char	*key, int *exists);
int			ft_add_to_env(t_mshell_sack *sack, char *key_val);

//enviroment_extra

int			ft_remove_env(t_mshell_sack *sack, char *key);
void		ft_envp_tidy(t_mshell_sack *sack);

//here_doc

int			ft_heredoc(char **str, int *i);
int			ft_expand_heredoc(int o_fd, t_redir_tok *tok, char **envp);

//history

void		ft_init_history_file(t_mshell_sack *sack);
void		ft_fill_history(t_mshell_sack *sack);
void		ft_add_history(char *str, t_mshell_sack *sack);

//init

void		init(t_mshell_sack *sack, char **envp, int argc, char **argv);

//lexer

t_list		*lexer(char	*str);
char		ft_isreserved(char c);

//lexer_utils

void		state_quote_delimiter(char *str, int *i, char delim);
void		skip_spaces(char *str, int *i, int *start);
int			ft_remove_quotes(t_list *tokens);
int			ft_exec_and_wait(t_cmdtree *tree_node, t_mshell_sack *sack);

//execute (2,3,4)

int			execute(t_cmdtree *tree_node, t_mshell_sack *sack);
int			expand_execute(t_cmdtree *tree_node, t_mshell_sack *sack);
int			ft_parse_and_exec(t_cmdtree *tree_node, t_mshell_sack *sack);
char		*ft_remove_brackets(char *str);
void		logic_expansion(t_cmdtree *tree_node);
void		ft_remove_outer_brackets(char *str);
int			get_log_expandible(char *str);
void		is_quote(char *str, int *i, int *last);
int			if_mask(char *str, int last);
char		*ft_get_left_token(char *str);
char		*ft_get_right_token(char *str);
void		ft_brackets(char *str, int *i);
char		ft_has_brackets(char *str);

//execute_utils (1)

void		ft_fork(t_cmd_node *node);
t_cmd_node	*ft_execute_lst(t_cmdtree *tree_node,
				t_mshell_sack *sack, int *last_pid);
int			ft_file_redirs(t_list *files, int input_fd, int output_fd,
				char **envp);
int			ft_exec_first_cmd(t_cmd_node *node, t_mshell_sack *sack,
				int *outfd);
int			exec_first_management(t_cmd_node *node, t_mshell_sack *sack,
				char **path);
int			ft_exec_single_cmd(t_cmd_node *node, t_mshell_sack *sack);
int			ft_no_path(t_cmd_node *node, int close, int fd);

//expander

char		*ft_expand(char *line, char **envp, char expand_all);

//redirect_utils

int			ft_open(t_redir_tok *f_tok);
void		ft_close(int fd);
void		ft_dup2(int oldfd, int newfd);

//syntax_checker

int			ft_check_syntax_heredoc(t_mshell_sack *sack);

//path_finder

char		*extract_exec_path(t_mshell_sack *sack, t_cmd_node *node);
void		ft_execbuiltin(t_cmd_node *node, t_mshell_sack *sack, char parent);
//debug only

void		print_tokens(t_list *tokens);

//term_settings

int			ft_set_signal_print(int val);

//utils

void		ft_printexit(int exit_code, t_mshell_sack *sack, char print);
void		ft_free_array(char **array);
void		free_cmd_tok(void *tok);
char		*get_cwd_str(void);
int			ft_isspace(char c);
void		ft_memory_err_exit(t_mshell_sack *sack);

//wildcards

char		*ft_expand_wildcards(char *str);
int			if_char_unop(char token, int index);
int			strlen_and_free(char **tmp, char **tmp2, char **file);

//wildcards_utils

void		get_files_init(DIR **dir_ptr, struct dirent **directory,
				char ***matches, int *ctr);

#endif
