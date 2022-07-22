#include "../includes/minishell.h"

// test code
void print_token(t_token *token, int flag)
{
	char type[7][10] = {"CMD", "PIPE", "TRUNC", "APPEND", "INPUT", "HEREDOC", "END"};
	while (token && flag == 1)
	{
		printf("[%s : %s]", token->value, type[token->type - 1]);
		token = token->nxt;
	}
	while (token && flag == 0)
	{
		printf("[%s]", token->value);
		token = token->nxt;
	}
	printf("\n");
}

// test code
void print_node(t_node *node)
{
	int i;
	char type[7][10] = {"CMD", "PIPE", "TRUNC", "APPEND", "INPUT", "HEREDOC", "END"};

	while (node)
	{
		i = 0;
		printf("-------- %s ----------\n", type[node->type - 1]);
		while (node->cmd && node->cmd[i])
		{
			printf("%s\n", node->cmd[i]);
			i++;
		}
		printf("infile : %d\n", node->fd[IN]);
		printf("outfile : %d\n", node->fd[OUT]);
		node = node->nxt;
	}
}

// test code
void print_heredoc(t_node *node)
{
	char *line;

	if (!node)
		return;

	while (node)
	{
		if (node->type == HEREDOC)
		{
			line = get_next_line(node->fd[IN]);
			printf("%d\n", node->fd[IN]);
			printf("------- heredoc -------\n");
			while (line)
			{
				printf("line : %s\n", line);
				free(line);
				line = get_next_line(node->fd[IN]);
			}
		}
		if (node)
			node = node->nxt;
	}
}

// test code
void print_tmpfiles()
{
	t_list *tmp = tmp_files(NULL, GET);
	printf("------ tmps files ------\n");
	while (tmp)
	{
		printf("%s\n", tmp->value);
		tmp = tmp->nxt;
	}
}

void check_redirection(t_node *node)
{
	t_node *tmp;

	while (node)
	{
		if (node->type == TRUNC || node->type == APPEND)
		{
			tmp = node->nxt;
			while (tmp)
			{
				if (tmp->type == PIPE)
					break;
				if (tmp->type == CMD)
					tmp->fd[OUT] = node->fd[OUT];
				tmp = tmp->nxt;
			}
		}
		node = node->nxt;
	}
}