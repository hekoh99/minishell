#include "../includes/minishell.h"

t_token *trim_space(char *line)
{
    t_token *head;
    t_token *new;
    t_token *tmp;
    int squote = 0;
    int dquote = 0;
    int start;
    int i = 0;

    start = 0;
    head = NULL;
    while ((line[i] != '\0' || i - start > 0))
    {
        if (line[i] == '\'' && squote == 0 && dquote == 0)
            squote = 1;
        if (line[i] == '\"' && squote == 0 && dquote == 0)
            dquote = 1;
        if (line[i] == '\'' && squote == 1)
            squote = 0;
        if (line[i] == '\"' && dquote == 1)
            dquote = 0;
        if ((line[i] == ' ' || line[i] == '\0') && (squote == 0 && dquote == 0))
        {
            printf("i : %d\n", i);
            if (i - start > 0)
            {
                new = malloc(sizeof(t_token));
                new->value = ft_substr(line, start, i - start);
                new->nxt = NULL;
                if (head == NULL)
                {
                    head = new;
                    head->prev = NULL;
                    tmp = head;
                }
                else
                {
                    tmp->nxt = new;
                    new->prev = tmp;
                    tmp = new;
                }
            }
            start = i + 1;
            if (line[i] == '\0')
                break;
        }
        if (line[i] != '\0')
            i++;
    }
    return (head);
}

t_token *expand(t_token *token) // parse $
{
    token += 0;
    return (0);
}