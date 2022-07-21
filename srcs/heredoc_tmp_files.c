#include "../includes/minishell.h"

extern int g_stat;

t_list *tmp_files(char *filename, int cmd)
{
    static t_list *head;

    if (cmd == GET)
        return (head);
    else if (cmd == ADD)
        head = add_files(head, filename);
    else if (cmd == DEL)
    {
        delete_files(head);
        head = NULL;
        return head;
    }
    return (head);
}

t_list *add_files(t_list *head, char *filename)
{
    t_list *newfile;
    t_list *tmp;

    newfile = (t_list *)malloc(sizeof(t_list));
    newfile->value = filename;
    newfile->nxt = NULL;
    if (head == NULL)
        head = newfile;
    else
    {
        tmp = head;
        while (tmp && tmp->nxt)
            tmp = tmp->nxt;
        tmp->nxt = newfile;
    }
    return (head);
}

void delete_files(t_list *head)
{
    t_list *target;
    t_list *tmp;

    tmp = head;
    while (tmp)
    {
        target = tmp;
        unlink(target->value);
        free(target->value);
        tmp = tmp->nxt;
        free(target);
    }
}