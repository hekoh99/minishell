#include "../../includes/utils.h"
#include "../../includes/minishell.h"

t_token *ft_dellist(t_token *head, char *target)
{
    t_token *tmp;
    t_token *del;

    tmp = head;
    while (tmp)
    {
        if (ft_strncmp(tmp->value, target, ft_strlen(target) + 1) == 0)
        {
            if (tmp->prev == NULL)
            {
                head = tmp->nxt;
                free(tmp->value);
                free(tmp);
                tmp = head;
            }
            else if (tmp->nxt == NULL)
            {
                tmp = tmp->prev;
                free(tmp->nxt->value);
                free(tmp->nxt);
                tmp->nxt = NULL;
            }
            else
            {
                del = tmp;
                tmp->prev->nxt = tmp->nxt;
                tmp->nxt->prev = tmp->prev;
                tmp = tmp->prev;
                free(del->value);
                free(del);
            }
        }
        tmp = tmp->nxt;
    }
    return (head);
}

void free_token_all(t_token *head)
{
    t_token *tmp;
    t_token *target;

    if (!head)
        return ;
    tmp = head;
    while (tmp)
    {
        target = tmp;
        if (target->value)
        {
            free(target->value);
        }
        tmp = tmp->nxt;
        free(target);
    }
}

void free_node_all (t_node *head)
{
    t_node *tmp;
    t_node *target;
    int i;

    if (!head)
        return ;

    tmp = head;
    while (tmp)
    {
        target = tmp;
        if (target->fd[IN] != -1 && target->fd[IN] != 0)
            close(target->fd[IN]);
        if (target->fd[OUT] != -1 && target->fd[OUT] != 1)
            close(target->fd[OUT]);
        if (target->cmd)
        {
            i = 0;
            while (target->cmd[i])
            {
                free(target->cmd[i]);
                i++;
            }
            free(target->cmd);
        }
        tmp = tmp->nxt;
        free(target);
    }
}