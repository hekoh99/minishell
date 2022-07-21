#include "../includes/minishell.h"

extern int g_stat;

static t_token *join_list_center(t_token *pos, int sep_size, int *index, int size)
{
    t_token *seperated;
    char *tmp;

    seperated = malloc(sizeof(t_token) * sep_size);
    seperated->value = ft_substr(pos->value, *index, sep_size);
    seperated->nxt = pos->nxt;
    pos->nxt->prev = seperated;
    pos->nxt = seperated;
    seperated->prev = pos;
    pos = pos->nxt; // 구분자 블록
    seperated = malloc(sizeof(t_token));
    seperated->value = ft_substr(pos->prev->value, *index + sep_size, size);
    seperated->nxt = pos->nxt;
    pos->nxt->prev = seperated;
    pos->nxt = seperated;
    seperated->prev = pos;
    tmp = pos->prev->value;
    pos->prev->value = ft_substr(tmp, 0, *index);
    free(tmp);
    if (sep_size == 2)
        (*index)++;
    return (pos);
}

static t_token *join_list_back(t_token *pos, int sep_size, int *index, int size)
{
    t_token *seperated;
    char *tmp;

    seperated = malloc(sizeof(t_token));
    seperated->value = ft_substr(pos->value, *index, sep_size);
    pos->nxt = seperated;
    seperated->prev = pos;
    seperated->nxt = NULL;
    pos = pos->nxt;
    seperated = malloc(sizeof(t_token));
    seperated->value = ft_substr(pos->prev->value, *index + sep_size, size);
    pos->nxt = seperated;
    seperated->prev = pos;
    seperated->nxt = NULL;
    tmp = pos->prev->value;
    pos->prev->value = ft_substr(tmp, 0, *index);
    free(tmp);
    if (sep_size == 2)
        (*index)++;
    return (pos);
}

static t_token *do_split_by_seps(t_token *pos, int sep_size, int *index, int size)
{
    if (pos->nxt != NULL)
    {
        pos = join_list_center(pos, sep_size, index, size);
    }
    else
    {
        pos = join_list_back(pos, sep_size, index, size);
    }
    return (pos);
}

static t_token *split_target_token(t_token *token, t_token *tmp, int *squote, int *dquote)
{
    int i;
    char *sep = ";|><";
    int sep_size;
    int size;

    i = 0;
    size = ft_strlen(tmp->value);
    while (tmp->value[i] != '\0')
    {
        check_quote(tmp->value[i], squote, dquote);
        if (ft_strchr(sep, tmp->value[i]) != 0 && *squote == 0 && *dquote == 0)
        {
            sep_size = check_duple_sep(tmp->value, i); // 0 이면 ||, ;; -> error
            if (sep_size == 0)
            {
                free_token_all(token);
                return (NULL);
            }
            tmp = do_split_by_seps(tmp, sep_size, &i, size);
        }
        i++;
    }
    return (tmp);
}

t_token *split_by_sep(t_token *token) // 연속된 구분자도 체크 완
{
    int i;
    t_token *tmp;
    int squote = 0;
    int dquote = 0;

    tmp = token;
    while (tmp)
    {
        tmp = split_target_token(token, tmp, &squote, &dquote);
        if (!tmp)
            return (NULL);
        tmp = tmp->nxt;
    }
    token = ft_dellist(token, "");
    return (token);
}