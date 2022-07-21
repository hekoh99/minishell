#include "../includes/minishell.h"

extern int g_stat;

static void set_expanded_value(t_token *token, char *replaced, int start, int *index)
{
    char *head;
    char *tail;

    if (token->value[start] == '?') // status 실제값으로 대체 완
    {
        replaced = ft_strdup(ft_itoa(g_stat));
        (*index) = start + 1;
    }
    if (!replaced)
        replaced = ft_strdup("");
    head = ft_substr(token->value, 0, start - 1);
    tail = ft_substr(token->value, *index, ft_strlen(token->value));
    free(token->value);
    token->value = ft_strjoin(head, replaced);
    token->value = ft_strjoin(token->value, tail);
    free(replaced);
    free(head);
    free(tail);
    if (ft_strlen(token->value) == 0) // 환경변수가 없으며 출력할 문자도 없을 때
    {
        free(token->value);
        token->value = NULL;
    }
    (*index) = (*index) - ((*index) - start + 1);
}

t_token *expand(t_token *token, t_env *env) // parse $ ~ 작은 따옴표 안은 무시
{
    int i;
    int squote;
    int dquote;
    int start;
    char *target;
    char *replaced;
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        i = 0;
        squote = 0;
        dquote = 0;
        while (tmp->value && tmp->value[i] != '\0')
        {
            check_quote(tmp->value[i], &squote, &dquote);
            if (tmp->value[i] == '$' && tmp->value[i + 1] != '\0' && squote == 0)
            {
                start = i + 1;
                i++;
                while ((ft_isalpha(tmp->value[i]) == 1 || ft_isdigit(tmp->value[i])) && tmp->value[i] != '\0')
                    i++;
                target = ft_substr(tmp->value, start, i - start);
                replaced = search_env(env, target);
                free(target);
                set_expanded_value(tmp, replaced, start, &i);
                // i--; // 반복문 후 i는 구분자 위치 또는 문자열의 끝에 위치
            }
            else if (i == 0 && tmp->value[i] == '~' && (ft_strlen(tmp->value) == 1 || tmp->value[i + 1] == '/') && squote == 0)
            {
                replaced = search_env(env, "HOME");
                if (!replaced) // HOME 없을 때
                    replaced = ft_strdup(getenv("HOME"));
                i++;
                set_expanded_value(tmp, replaced, 1, &i);
            }
            else
                i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}