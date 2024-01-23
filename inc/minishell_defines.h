/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_defines.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guortun- <guortun-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 17:46:10 by sacorder          #+#    #+#             */
/*   Updated: 2024/01/23 16:12:26 by guortun-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEFINES_H
# define MINISHELL_DEFINES_H
# define PROMPT_BASE "µ\001\x1b[32m\002Shell\001\x1b[0m\002 - \001\x1b[31m\002"
# define INFILE_MASK 0b1
# define OUTFILE_MASK 0b10
# define HEREDOC_MASK 0b100
# define CONCATOUT_MASK 0b1000
# define FILE_REDIR 0
# define PIPE 1
# define LOGIC 2
# define ARG 3
# define E_EXP_ARG 4
# define W_EXP_ARG 5
# define AND_MASK 0b1
# define OR_MASK 0b10
# define WAIT_MASK 0b100
# define H_DOC_TMP_BASE "/tmp/.dash_tmp_heredoc_"
# define HIST_TMP_BASE "/.dash_history"
# define HIST_ERROR_MSG "Minishell: HOME is not set, history unavailable."
# ifndef SIZE_T_MAX
#  define SIZE_T_MAX UINT_MAX
# endif
/* COLORS */
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RST "\033[0m"
# define BOLD "\033[1m"

# define DEBUG 0
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
