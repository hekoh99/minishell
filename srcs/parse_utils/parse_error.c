/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:21:46 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 14:44:33 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

t_token	*open_quote_err(t_token *head)
{
	printf("minishell: open quotation syntax error\n");
	g_stat = SYNTAX;
	free_token_all(head);
	return (NULL);
}

int	print_syntax_error(t_token **token, const char *msg)
{
	printf("minishell: %s", msg);
	g_stat = SYNTAX;
	free_token_all(*token);
	*token = NULL;
	return (-1);
}

int	error_handler(t_node *head, t_token **token, t_token **tmp)
{
	if ((*tmp)->type == PIPE)
	{
		if ((!(*tmp)->prev || (*tmp)->prev->type != CMD)
			|| (!(*tmp)->nxt || (*tmp)->nxt->type == PIPE))
			return (print_syntax_error(token,
									   "syntax error near unexpected token `|'\n"));
	}
	else if ((*tmp)->type == END)
	{
		if ((!(*tmp)->prev || (*tmp)->prev->type != CMD)
			|| ((*tmp)->nxt && (*tmp)->nxt->type == END))
			return (print_syntax_error(token,
									   "syntax error near unexpected token `;'\n"));
	}
	else if ((*tmp)->type == TRUNC || (*tmp)->type == APPEND
		|| (*tmp)->type == INPUT || (*tmp)->type == HEREDOC)
	{
		if ((!(*tmp)->nxt) || ((*tmp)->nxt->type > 1))
			return (print_syntax_error(token,
									   "syntax error near unexpected token `newline'\n"));
	}
	return (1);
}
