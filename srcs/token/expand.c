/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:25:48 by hako              #+#    #+#             */
/*   Updated: 2022/07/22 22:25:50 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

char *ft_strjoin_free_s1(char *s1, char *s2)
{
    char *tmp;

    tmp = s1;
    s1 = ft_strjoin(s1, s2);
    ft_free(tmp);
    return (s1);
}

static void set_expanded_value(t_token *token, char *replaced, int start, int *index)
{
    char *head;
    char *tail;

    if (token->value[start] == '?') // status 실제값으로 대체 완
    {
        replaced = ft_itoa(g_stat);
        (*index) = start + 1;
    }
    if (!replaced)
        replaced = ft_strdup("");
    head = ft_substr(token->value, 0, start - 1);
    tail = ft_substr(token->value, *index, ft_strlen(token->value));
    free(token->value);
    token->value = ft_strjoin(head, replaced);
    token->value = ft_strjoin_free_s1(token->value, tail);
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

void expand_home_var(t_token *tmp, t_env *env, int *index)
{
    char *replaced;

    replaced = search_env(env, "HOME");
    if (!replaced) // HOME 없을 때
        replaced = ft_strdup(getenv("HOME"));
    (*index)++;
    set_expanded_value(tmp, replaced, 1, index);
}

void expand_env_var(t_token *tmp, t_env *env, int *index)
{
    int start;
    char *target;
    char *replaced;

    start = (*index) + 1;
    (*index)++;
    while ((ft_isalpha(tmp->value[*index]) == 1 || ft_isdigit(tmp->value[*index])) && tmp->value[*index] != '\0')
        (*index)++;
    target = ft_substr(tmp->value, start, (*index) - start);
    replaced = search_env(env, target);
    free(target);
    set_expanded_value(tmp, replaced, start, index);
}

t_token *expand(t_token *token, t_env *env) // parse $ ~ 작은 따옴표 안은 무시
{
    int i;
    int squote;
    int dquote;
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
                expand_env_var(tmp, env, &i);
            else if (i == 0 && tmp->value[i] == '~' && (ft_strlen(tmp->value) == 1 || tmp->value[i + 1] == '/') && squote == 0)
                expand_home_var(tmp, env, &i);
            else
                i++;
        }
        tmp = tmp->nxt;
    }
    return (token);
}
