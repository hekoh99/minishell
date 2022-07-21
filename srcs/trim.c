#include "../includes/minishell.h"

extern int g_stat;

t_token *trim_space(char *line)
{
    t_token *head;
    int squote;
    int dquote;
    int start;
    int i;

    start = 0;
    i = 0;
    dquote = 0;
    squote = 0;
    head = NULL;
    while ((line[i] != '\0' || i - start > 0))
    {
        check_quote(line[i], &squote, &dquote);
        if ((line[i] == ' ' || line[i] == '\0') && (squote == 0 && dquote == 0))
        {
            if (i - start > 0)
                head = add_token(head, ft_substr(line, start, i - start));
            start = i + 1;
            if (line[i] == '\0')
                break;
        }
        if (line[i] != '\0')
            i++;
        if (line[i] == '\0' && squote + dquote > 0)
            break;
    }
    if (squote == 1 || dquote == 1)
        return (open_quote_err(head));
    return (head);
}

char *inside_quote(char *value, int start, int *mid)
{
    char *inside;
    char *head;
    char *tail;
    char *str;

    str = ft_substr(value, start + 1, *mid - start - 1);
    head = ft_substr(value, 0, start);
    tail = ft_substr(value, *mid + 1, ft_strlen(value));
    free(value);
    inside = ft_strjoin(head, str);
    *mid = ft_strlen(inside) - 1;
    inside = ft_strjoin(inside, tail);
    free(str);
    free(head);
    free(tail);

    return (inside);
}

void do_trim_quote(t_token *tmp)
{
    int i;
    int squote;
    int dquote;
    int start;

    i = 0;
    squote = 0;
    dquote = 0;
    start = -1;
    while (tmp->value && tmp->value[i] != '\0')
    {
        check_quote(tmp->value[i], &squote, &dquote);
        if (tmp->value[i] == '\'' || tmp->value[i] == '\"')
        {
            if ((squote == 1 || dquote == 1) && start == -1)
                start = i;
            else if (start != -1 && dquote == 0 && squote == 0)
            {
                tmp->value = inside_quote(tmp->value, start, &i);
                start = -1;
            }
        }
        i++;
    }
}

t_token *trim_quote(t_token *token)
{
    t_token *tmp;
    int i;
    char *str;
    int squote;
    int dquote;
    int start;

    tmp = token;
    while (tmp)
    {
        i = 0;
        squote = 0;
        dquote = 0;
        start = 0;
        while (tmp->value && tmp->value[i] != '\0')
        {
            if (tmp->value[i] == '\'' && squote == 0 && dquote == 0)
            {
                start = i;
                squote = 1;
            }
            else if (tmp->value[i] == '\"' && squote == 0 && dquote == 0)
            {
                start = i;
                dquote = 1;
            }
            else if (tmp->value[i] == '\'' && squote == 1)
            {
                squote = 0;
                tmp->value = inside_quote(tmp->value, start, &i);
            }
            else if (tmp->value[i] == '\"' && dquote == 1)
            {
                dquote = 0;
                tmp->value = inside_quote(tmp->value, start, &i);
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}
