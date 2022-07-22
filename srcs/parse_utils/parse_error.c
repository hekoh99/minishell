/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:21:46 by hako              #+#    #+#             */
/*   Updated: 2022/07/22 22:21:48 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

t_token *open_quote_err(t_token *head)
{
    printf("minishell: open quotation syntax error\n");
    g_stat = SYNTAX;
    free_token_all(head);
    return (NULL);
}

t_node *print_syntax_error(t_token **token, const char *msg)
{
    printf("%s", msg);
    g_stat = SYNTAX;
    free_token_all(*token);
    *token = NULL;
    return (NULL);
}

t_node *error_handler(t_node *head, t_token **token, t_token **tmp)
{
    if ((*tmp)->type == PIPE)
    {
        if (!(*tmp)->prev || (*tmp)->prev->type != CMD)
            return (print_syntax_error(token, "minishell: syntax error near unexpected token `|'\n"));
        if (!(*tmp)->nxt || (*tmp)->nxt->type == PIPE)
            return (print_syntax_error(token, "minishell: syntax error near unexpected token `|'\n"));
    }
    else if ((*tmp)->type == END)
    {
        if (!(*tmp)->prev || (*tmp)->prev->type != CMD)
            return (print_syntax_error(token, "minishell: syntax error near unexpected token `;'\n"));
        if ((*tmp)->nxt && (*tmp)->nxt->type == END)
            return (print_syntax_error(token, "minishell: syntax error near unexpected token `;'\n"));
    }
    else if ((*tmp)->type == TRUNC || (*tmp)->type == APPEND || (*tmp)->type == INPUT || (*tmp)->type == HEREDOC)
    {
        if (!(*tmp)->nxt)
            return (print_syntax_error(token, "minishell: syntax error near unexpected token `newline'\n"));
        if ((*tmp)->nxt->type > 1)
            return (print_syntax_error(token, "minishell: syntax error near unexpected token `newline'\n"));
    }
    return (head);
}
