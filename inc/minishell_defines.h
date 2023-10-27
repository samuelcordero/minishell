/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_defines.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacorder <sacorder@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 17:46:10 by sacorder          #+#    #+#             */
/*   Updated: 2023/10/27 13:33:40 by sacorder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEFINES_H
# define MINISHELL_DEFINES_H
# define MAX_SIZE 65535
# define PROMPT_BASE "MiniShell - "
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
# define H_DOC_TMP_BASE "/tmp/dash_tmp_heredoc_"

typedef struct s_cmdtoken
{
	char	*str;
	int		type;
}	t_cmdtoken;

typedef struct s_redir_tok
{
	char				*file_name;
	char				redir_type;
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
	t_cmd_node			*cmd_list;
	char				is_logic;
	char				logic_op;
	int					exit_code;
}	t_cmdtree;

typedef struct s_mshell_sack
{
	t_list		*cmd_tokens;
	t_cmdtree	*cmd_tree;
	char		*line;
	char		**envp;
	long		env_size;
	long		env_elems;
	char		*custom_prompt;
	char		eof;
}	t_mshell_sack;

#endif
