/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:28:58 by hako              #+#    #+#             */
/*   Updated: 2022/07/21 14:15:32 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

extern int g_stat;

t_token *add_token(t_token *head, char *value)
{
    t_token *new;
    t_token *tmp;

    new = (t_token *)malloc(sizeof(t_token));
    new->value = value;
    new->nxt = NULL;
    new->prev = NULL;
    if (head == NULL)
    {
        head = new;
        head->prev = NULL;
        tmp = head;
    }
    else
    {
        tmp = head;
        while (tmp->nxt)
            tmp = tmp->nxt;
        tmp->nxt = new;
        new->prev = tmp;
    }
    return (head);
}

t_token *add_type(t_token *token)
{
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        if (tmp->value[0] == '>')
        {
            tmp->type = TRUNC;
            if (tmp->value[1] == '>')
                tmp->type = APPEND;
        }
        else if (tmp->value[0] == '<')
        {
            tmp->type = INPUT;
            if (tmp->value[1] == '<')
                tmp->type = HEREDOC;
        }
        else if (tmp->value[0] == '|')
            tmp->type = PIPE;
        else if (tmp->value[0] == ';')
            tmp->type = END;
        else
            tmp->type = CMD;
        tmp = tmp->nxt;
    }
    return (token);
}

t_node *add_cmd_arr(t_node *new, t_token *target, int iter)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < iter)
    {
        if (target->value != NULL)
        {
            new->cmd[j] = ft_strdup(target->value);
            j++;
        }
        target = target->nxt;
        i++;
    }
    new->cmd[j] = NULL;
    return (new);
}

void connect_cmd(t_token **head, t_token *cmd, t_token *arg_start, t_token *redir)
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

t_token *do_reorder_token(t_token **head, t_token *file, t_token *redir, t_token *cmd)
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