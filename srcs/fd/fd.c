#include "../../includes/minishell.h"

extern int g_stat;

t_node *set_cmd_pipe(t_node *prev, t_node *tmp, t_node *cmd)
{
    prev->fd[OUT] = tmp->fd[OUT];
    if (cmd && cmd->fd[OUT] == 1) // out이 정해지지 않은 상태에서 파이프를 만나면
        cmd->fd[OUT] = tmp->fd[OUT];
    if (!cmd)
        close(tmp->fd[OUT]);
    return (NULL);
}

void set_cmd_output(t_node *tmp, t_node *cmd)
{
    if (tmp->nxt && tmp->nxt->type == PIPE)
        ft_close(tmp->nxt->fd[OUT]);
    if (cmd)
        cmd->fd[OUT] = tmp->fd[OUT];
}

t_node *init_cmd(t_node *prev, t_node *cmd)
{
    if (prev && prev->type > CMD)
        cmd->fd[IN] = prev->fd[IN];
    if (cmd->nxt == NULL)
        cmd->fd[OUT] = 1;
    return (cmd);
}

int set_cmd_fd(t_node *node)
{
    t_node *tmp;
    t_node *prev;
    t_node *cmd;

    tmp = node;
    prev = NULL;
    cmd = NULL;
    while (tmp)
    {
        if (prev && prev->type == END)
            prev = NULL;
        if (tmp->type == CMD)
            cmd = init_cmd(prev, tmp);
        if (cmd && (tmp->type == INPUT || tmp->type == HEREDOC))
            cmd->fd[IN] = tmp->fd[IN];
        if (tmp->type == TRUNC || tmp->type == APPEND)
            set_cmd_output(tmp, cmd);
        if (prev && tmp->type == PIPE)
            cmd = set_cmd_pipe(prev, tmp, cmd);
        prev = tmp;
        tmp = tmp->nxt;
    }
    return (1);
}

t_node *get_fd(t_node *node)
{
    if (!set_separator_fd(node))
        return (NULL);
    set_cmd_fd(node);
    return (node);
}