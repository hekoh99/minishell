/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:28:58 by hako              #+#    #+#             */
/*   Updated: 2022/07/22 22:26:26 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void add_redir_type(t_token *token)
{
    if (token->value[0] == '>')
    {
        token->type = TRUNC;
        if (token->value[1] == '>')
            token->type = APPEND;
    }
    else if (token->value[0] == '<')
    {
        token->type = INPUT;
        if (token->value[1] == '<')
            token->type = HEREDOC;
    }
}

t_token *add_type(t_token *token)
{
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        if (tmp->value[0] == '>' || tmp->value[0] == '<')
            add_redir_type(tmp);
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
