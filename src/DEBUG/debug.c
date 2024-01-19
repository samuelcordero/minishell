#include "minishell.h"

static void ternary(char is_builtin)
{
	DEBUG_TRACE_ENTER
	if (is_builtin)
	{
		DEBUG_INT(is_builtin);
	}
	else
	{
		DEBUG_INT(is_builtin);
	}
	printf("\n-----------\n\n");
}

void	print_tokens(t_list *tokens)
{
	t_list		*current;
	t_cmdtoken	*ccontent;
	int			counter;

	current = tokens;
	counter = 0;
	while (current)
	{
		ccontent = (t_cmdtoken *)current->content;
		DEBUG_INT(counter);
		if (ccontent)
		{
			DEBUG_STR(ccontent->str);
			DEBUG_INT(ccontent->type);
		}
		else
			DEBUG_STR("NULL\n");
		DEBUG_STR("--------------\n\n");
		counter++;
		current = current->next;
	}
}

static void	print_cmd_node(t_cmd_node *lst)
{
	t_list		*redirs_lst;
	int			i;

	if (lst)
		printf("\n\nCommand list:\n\n");
	while (lst)
	{
		i = -1;
		while (lst->args[++i])
			printf("Arg %i: %s\n", i, lst->args[i]);
		redirs_lst = lst->redirs_lst;
		while (redirs_lst)
		{
			printf("File redir type: %i, filename %s\n",
			(int)((t_redir_tok *)redirs_lst->content)->redir_type,
			((t_redir_tok *)redirs_lst->content)->file_name);
			redirs_lst = redirs_lst->next;
		}
		ternary(lst->is_builtin);
		lst = lst->next;
	}
}
void	print_cmdtree(t_cmdtree *head)
{
	if ((int)head->is_logic == OR_MASK)
		printf("\n-----OR NODE-----\n");
	else if ((int)head->is_logic == AND_MASK)
		printf("\n-----AND NODE-----\n");
	else if ((int)head->is_logic == WAIT_MASK)
		printf("\n-----WAIT NODE-----\n");
	if (head->left)
	{
		printf("<LEFT (MAIN) BRANCH:>");
		print_cmdtree(head->left);
	}
	if (head->right)
	{
		printf("<RIGHT (OPTIONAL) BRANCH:>");
		print_cmdtree(head->right);
	}
	print_cmd_node(head->cmd_list);
}