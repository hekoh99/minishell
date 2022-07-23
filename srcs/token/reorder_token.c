/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:25:57 by hako              #+#    #+#             */
/*   Updated: 2022/07/22 22:25:59 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

static void connect_cmd(t_token **head, t_token *cmd, t_token *arg_start, t_token *redir)
{
    if (cmd)
    {
        cmd->nxt = arg_start;
        arg_start->prev = cmd;
    }
    else // cmd가 null 일때
    {
        if (!redir->prev)
            *head = arg_start;
        else
            redir->prev->nxt = arg_start;
    }
}

static t_token *do_reorder_token(t_token **head, t_token *file, t_token *redir, t_token *cmd)
{
    t_token *arg_start;
    t_token *arg_end;
    t_token *tmp;

    arg_start = file->nxt;
    tmp = arg_start;
    while (tmp && tmp->type == CMD)
    {
        arg_end = tmp;
        tmp = tmp->nxt;
    }
    file->nxt = tmp; // redir의 뒷단을 tmp에 연결
    if (tmp)
        tmp->prev = file;
    connect_cmd(head, cmd, arg_start, redir);
    arg_end->nxt = redir;
    redir->prev = arg_end;
    return (arg_end);
}

t_token *reorder_token(t_token *token)
{
    t_token *tmp;
    t_token *cmd;
    t_token *redir;
    t_token *file;

    tmp = token;
    cmd = NULL;
    while (tmp)
    {
        if (tmp->type == CMD) // 명령어
            cmd = tmp;
        else if (tmp->type == PIPE || tmp->type == END)
            cmd = NULL;
        else // redir 일 때
        {
            redir = tmp;
            file = tmp->nxt;
            if (!file)
                print_syntax_error(&token, "minishell: syntax error near unexpected token `newline'\n");
            if (file && file->nxt && file->nxt->type == CMD) // redir 뒷 단이 cmd면
                tmp = do_reorder_token(&token, file, redir, cmd);
        }
        tmp = tmp->nxt;
    }
    return (token);
}
