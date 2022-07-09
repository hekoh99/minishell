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

    tmp = head;
    while (tmp)
    {
        tmp = tmp->nxt;
        free(tmp->prev->value);
        free(tmp->prev);
    }
}