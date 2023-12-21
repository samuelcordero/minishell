/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_defines.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 17:46:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/12/21 13:54:17 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEFINES_H
# define MINISHELL_DEFINES_H
# define PROMPT_BASE "µ\001\x1b[32m\002Shell\001\x1b[0m\002 - \001\x1b[31m\002"
# define INFILE_MASK 0b1
# define OUTFILE_MASK 0b10
# define HEREDOC_MASK 0b100
# define CONCATOUT_MASK 0b1000
# define ARG 0
# define FILE_REDIR 1
# define PIPE 2
# define AND_MASK 0b1
# define OR_MASK 0b10
# define WAIT_MASK 0b100
# define H_DOC_TMP_BASE "/tmp/.dash_tmp_heredoc_"
# define HIST_TMP_BASE "/.dash_history"
# define HIST_ERROR_MSG "Minishell: HOME is not set, history unavailable."
# ifndef SIZE_T_MAX
#  define SIZE_T_MAX UINT_MAX
# endif

//NOT POSIX :c
# ifndef ECHOCTL
#  define ECHOCTL 0000400
# endif

typedef struct s_cmdtoken
{
	char	*str;
	int		type;
}	t_cmdtoken;

typedef struct s_redir_tok
{
	char	*file_name;
	char	redir_type;
}	t_redir_tok;

typedef struct s_cmd_node
{
	char				**args;
	char				pipe_out;
	t_list				*redirs_lst;
	struct s_cmd_node	*next;
	int					pid;
	int					exit_code;
	char				is_builtin;
	int					pipe_fds[2];
}	t_cmd_node;

typedef struct s_cmdtree
{
	struct s_cmdtree	*left;
	struct s_cmdtree	*right;
	t_list				*cmd_tokens;
	char				*cmd_str;
	char				*expanded_str;
	t_cmd_node			*cmd_list;
	char				is_logic;
	int					exit_code;
}	t_cmdtree;

typedef struct s_mshell_sack
{
	int			history_fd;
	int			last_exit;
	t_cmdtree	*cmd_tree;
	char		**envp;
	long		env_size;
	long		env_elems;
	char		*custom_prompt;
	char		eof;
}	t_mshell_sack;

#endif
