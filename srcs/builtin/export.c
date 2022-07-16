/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:53 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/16 15:45:46 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env *dup_env(t_env *env)
{
    t_env *dup_env;

    dup_env = (t_env *)ft_malloc(sizeof(t_env));
    if (!env)
    {
        dup_env->key = NULL;
        dup_env->value = NULL;
        dup_env->nxt = NULL;
    }
    else
    {
        dup_env->key = ft_strdup(env->key);
        dup_env->value = ft_strdup(env->value);
        dup_env->nxt = NULL;
    }
    return (dup_env);
}

t_env *dup_envp(t_env *envp)
{
    t_env *dup_envp;
    t_env *tmp;

    dup_envp = (t_env *)ft_malloc(sizeof(t_env));
    dup_envp = dup_env(envp);
    if (dup_envp->key)
    {
        tmp = dup_envp;
        while (envp->nxt)
        {
            envp = envp->nxt;
            tmp->nxt = (t_env *)ft_malloc(sizeof(t_env));
            tmp->nxt->key = ft_strdup(envp->key);
            tmp->nxt->value = ft_strdup(envp->value);
            tmp->nxt->nxt = NULL;
            tmp = tmp->nxt;
        }
    }
    return (dup_envp);
}

void swap_envp_var(t_env *left, t_env *right)
{
    t_env tmp;

    tmp.key = left->key;
    tmp.value = left->value;
    left->key = right->key;
    left->value = right->value;
    right->key = tmp.key;
    right->value = tmp.value;
}

t_env *sort_envp(t_env *envp)
{
    t_env *tmp;
    t_env *left;
    t_env *right;

    tmp = envp;
    while (tmp)
    {
        left = tmp;
        right = tmp->nxt;
        while (right)
        {
            if (ft_strncmp(left->key, right->key, select_longer(left->key, right->key)) > 0)
                swap_envp_var(left, right);
            right = right->nxt;
        }
        tmp = tmp->nxt;
    }
    return (envp);
}

void print_all_envp(t_env *envp, int has_prefix)
{
    while (envp)
    {
        if (ft_strlen(envp->key) == 1 && envp->key[0] == '_' && has_prefix)
        {
            envp = envp->nxt;
            continue;
        }
        if (has_prefix)
            printf("declare -x %s=\"%s\"\n", envp->key, envp->value);
        else
            printf("%s=%s\n", envp->key, envp->value);
        envp = envp->nxt;
    }
}

void print_sorted_envp(t_env *envp)
{
    t_env *sorted_envp;
    t_env *tmp;

    tmp = dup_envp(envp);
    tmp = sort_envp(tmp);
    print_all_envp(tmp, 1);
}

int find_sep(char *str, int sep)
{
    int i;

    i = 0;
    while (str && str[i])
    {
        if (str[i] == sep)
            return (i);
        ++i;
    }
    return (i);
}

t_env *make_env(char *key, char *value)
{
    t_env *env;

    env = (t_env *)ft_malloc(sizeof(t_env));
    env->key = ft_strdup(key);
    env->value = ft_strdup(value);
    env->nxt = NULL;
    return (env);
}

t_env *add_env(t_env *envp, t_env *env)
{
    t_env *tmp;

    tmp = envp;
    while (tmp->nxt)
        tmp = tmp->nxt;
    tmp->nxt = env;
    return (envp);
}

int is_duplicate_envp(t_env *envp, char *key)
{
    t_env *tmp;

    tmp = envp;
    while (tmp)
    {
        if (ft_strncmp(tmp->key, key, select_longer(tmp->key, key)) == 0)
            return (1);
        tmp = tmp->nxt;
    }
    return (0);
}

t_env *update_env(t_env *envp, char *key, char *value)
{
    t_env *tmp;

    tmp = envp;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            ft_free(tmp->value);
            tmp->value = ft_strdup(value);
            break;
        }
        tmp = tmp->nxt;
    }
    if (!tmp)
        envp = add_env(envp, make_env(key, value));
    return (envp);
}

t_env *ft_set(t_env *envp, char *key, char *value)
{
    envp = update_env(envp, key, value);
    ft_free(key);
    ft_free(value);
    return (envp);
}

void update_envp(t_node *node)
{
    int i;
    int sep;
    char *key;
    char *value;

    i = 0;
    sep = 1;
    while (node->cmd[++i])
    {
        sep = find_sep(node->cmd[i], '=');
        if (sep == 0)
        {
            print_error2(ft_strdup("export : `"), ft_strjoin(node->cmd[i], "': not a valid identifier\n"), 1);
            continue;
        }
        key = ft_substr(node->cmd[i], 0, sep);
        if (is_invalid_key("export", key) || sep == ft_strlen(node->cmd[i]))
            continue;
        value = ft_substr(node->cmd[i], sep + 1, ft_strlen(node->cmd[i]));
        node->envp = ft_set(node->envp, key, value);
    }
}

void ft_export(t_node *node)
{
    if (node->cmd[1] == NULL)
        print_sorted_envp(node->envp);
    else
        update_envp(node);
}
