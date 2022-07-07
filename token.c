#include "minishell.h"

t_token *trim_space(char *line)
{
    t_token *head;
    t_token *new;
    t_token *tmp;
    int squote = 0;
    int dquote = 0;
    int start;
    int i;

    start = 0;
    head = NULL;
    while (line[i] != '\0')
    { 
        if (line[i] == ' ')
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
                new->prev = tmp;
                tmp = new;
            }
        }
        i++;
    }
    return (head);
}

t_token *expand(t_token *token) // parse $
{

}