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
        else if (line[i] == '\"' && squote == 0 && dquote == 0)
            dquote = 1;
        else if (line[i] == '\'' && squote == 1)
            squote = 0;
        else if (line[i] == '\"' && dquote == 1)
            dquote = 0;
        if ((line[i] == ' ' || line[i] == '\0') && (squote == 0 && dquote == 0))
        {
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
        if (line[i] == '\0' && squote + dquote > 0)
            break;
    }
    if (squote == 1 || dquote == 1)
    {
        printf("open quotation error\n");
        free_token_all(head);
        head = NULL;
    }
    return (head);
}

t_token *split_by_sep(t_token *token) // 연속된 구분자 체크 해야함
{
    int i;
    t_token *seperated;
    t_token *tmp;
    int size;
    char *sep = ";|><";
    int squote = 0;
    int dquote = 0;

    tmp = token;
    while (tmp)
    {
        i = 0;
        size = ft_strlen(tmp->value);
        while (tmp->value[i] != '\0')
        {
            if (tmp->value[i] == '\'' && squote == 0 && dquote == 0)
                squote = 1;
            else if (tmp->value[i] == '\"' && squote == 0 && dquote == 0)
                dquote = 1;
            else if (tmp->value[i] == '\'' && squote == 1)
                squote = 0;
            else if (tmp->value[i] == '\"' && dquote == 1)
                dquote = 0;
            if (ft_strchr(sep, tmp->value[i]) != 0 && squote == 0 && dquote == 0)
            {
                seperated = malloc(sizeof(t_token));
                seperated->value = ft_substr(tmp->value, i, 1);
                if (tmp->nxt != NULL)
                {
                    // 첫번째 구분자 복사 후 리스트 병합
                    seperated->nxt = tmp->nxt;
                    tmp->nxt->prev = seperated;
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    tmp = tmp->nxt; // 구분자 블록
                    // 구분자 후단
                    seperated = malloc(sizeof(t_token));
                    seperated->value = ft_substr(tmp->prev->value, i + 1, size);
                    seperated->nxt = tmp->nxt;
                    tmp->nxt->prev = seperated;
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    // 구분자 전단
                    tmp->prev->value = ft_substr(tmp->prev->value, 0, i);
                }
                else
                {
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    seperated->nxt = NULL;
                    tmp = tmp->nxt;
                    seperated = malloc(sizeof(t_token));
                    seperated->value = ft_substr(tmp->prev->value, i + 1, size);
                    tmp->nxt = seperated;
                    seperated->prev = tmp;
                    seperated->nxt = NULL;
                    // 구분자 전단
                    tmp->prev->value = ft_substr(tmp->prev->value, 0, i);
                }
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    token = ft_dellist(token, "");
    return (token);
}

char *search_env(t_env *env, char *target)
{
    char *real_val;
    int i;
    int find;

    real_val = NULL;
    while (env)
    {
        if (ft_strncmp(env->key, target, select_longer(env->key, target)) == 0) // 환경변수 찾음
        {
            real_val = ft_substr(env->value, 0, ft_strlen(env->value));
            break;
        }
        env = env->nxt;
    }
    return (real_val);
}

t_token *expand(t_token *token, t_env *env) // parse $ ~ 작은 따옴표 안은 무시
{
    int i;
    int squote;
    int dquote;
    int start;
    char *target;
    char *replaced;
    char *sep = "$~";
    char *head;
    char *tail;
    t_token *tmp;

    tmp = token;
    while (tmp)
    {
        i = 0;
        squote = 0;
        dquote = 0;
        while (tmp->value[i] != '\0')
        {
            if (tmp->value[i] == '\'' && squote == 0 && dquote == 0)
                squote = 1;
            else if (tmp->value[i] == '\"' && squote == 0 && dquote == 0)
                dquote = 1;
            else if (tmp->value[i] == '\'' && squote == 1)
                squote = 0;
            else if (tmp->value[i] == '\"' && dquote == 1)
                dquote = 0;
            if (tmp->value[i] == '$' && squote == 0)
            {
                start = i + 1;
                i++;
                while (ft_strchr(sep, tmp->value[i]) == 0 && tmp->value[i] != '\0')
                    i++;
                target = ft_substr(tmp->value, start, i - start);
                replaced = search_env(env, target);
                free(target);

                if (replaced != NULL) // NULL 일 때 - 못 찾았을 때 처리하기
                {
                    head = ft_substr(tmp->value, 0, start - 1);
                    tail = ft_substr(tmp->value, i, ft_strlen(tmp->value));
                    free(tmp->value);
                    tmp->value = ft_strjoin(head, replaced);
                    tmp->value = ft_strjoin(tmp->value, tail);
                    free(replaced);
                    free(head);
                    free(tail);
                }
                else
                {
                    head = ft_substr(tmp->value, 0, start - 1);
                    tail = ft_substr(tmp->value, i, ft_strlen(tmp->value));
                    free(tmp->value);
                    tmp->value = ft_strjoin(head, tail);
                    free(head);
                    free(tail);
                }
                i--; // 반복문 후 i는 구분자 위치 또는 문자열의 끝에 위치
            }
            if (tmp->value[i] == '~' && ft_strlen(tmp->value) == 1 && squote == 0)
            {
                replaced = search_env(env, "HOME");
                free(tmp->value);
                tmp->value = replaced;
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}

t_token *trim_quote(t_token *token)
{
    t_token *tmp;
    int i;
    char *head;
    char *tail;
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
        while (tmp->value[i] != '\0')
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
                str = ft_substr(tmp->value, start + 1, i - start - 1);
                head = ft_substr(tmp->value, 0, start);
                tail = ft_substr(tmp->value, i + 1, ft_strlen(tmp->value));
                free(tmp->value);
                tmp->value = ft_strjoin(head, str);
                i = ft_strlen(tmp->value) - 1;
                tmp->value = ft_strjoin(tmp->value, tail);
                free(str);
                free(head);
                free(tail);
            }
            else if (tmp->value[i] == '\"' && dquote == 1)
            {
                dquote = 0;
                str = ft_substr(tmp->value, start + 1, i - start - 1);
                head = ft_substr(tmp->value, 0, start);
                tail = ft_substr(tmp->value, i + 1, ft_strlen(tmp->value));
                free(tmp->value);
                tmp->value = ft_strjoin(head, str);
                i = ft_strlen(tmp->value) - 1;
                tmp->value = ft_strjoin(tmp->value, tail);
                free(str);
                free(head);
                free(tail);
            }
            i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}
