/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:53 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/11 20:52:35 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env *init_env(char **env)
{
    int i = 0;
    t_env *lst;
    t_env *new;
    t_env *tmp;
    int j;

    lst = NULL;
    tmp = lst;
    while (env[i])
    {
        j = 0;
        while (env[i][j] != '=')
            j++;
        new = (t_env *)ft_malloc(sizeof(t_env));
        new->key = ft_substr(env[i], 0, j);
        new->value = ft_substr(env[i], j + 1, ft_strlen(env[i]));
        new->nxt = NULL;
        if (!lst)
        {
            lst = new;
            tmp = lst;
            i++;
            continue;
        }
        else
            tmp->nxt = new;
        tmp = tmp->nxt;
        i++;
    }
    return (lst);
}

int is_invalid_key(char *key, int *exit_status)
{
    int i;

    *exit_status = 0;
    if (ft_strlen(key) == 1 && key[0] == '_')
        return (0);
    i = -1;
    while (key[++i])
    {
        if (!ft_isalpha(key[i]) && key[i] != '_')
        {
            if (i > 0 && ft_isdigit(key[i]))
                continue;
            ft_free(key);
            *exit_status = 1;
            return (1);
        }
    }
    return (0);
}

int select_bigger(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

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
            if (ft_strncmp(left->key, right->key, select_bigger(ft_strlen(left->key), ft_strlen(right->key))) > 0)
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
        if (ft_strlen(envp->key) == 1 && envp->key[0] == '_')
        {
            envp = envp->nxt;
            continue;
        }
        if (has_prefix)
            printf("declare -x ");
        printf("%s=\"%s\"\n", envp->key, envp->value);
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
    return (-1);
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
        if (ft_strncmp(tmp->key, key, select_bigger(ft_strlen(tmp->key), ft_strlen(key))) == 0)
            return (1);
        tmp = tmp->nxt;
    }
    return (0);
}

t_env *update_env(t_env *envp, char *key, char *value)
{
    while (envp)
    {
        if (ft_strncmp(envp->key, key, select_bigger(ft_strlen(envp->key), ft_strlen(key))) == 0)
        {
            ft_free(envp->value);
            envp->value = ft_strdup(value);
            break;
        }
        envp = envp->nxt;
    }
    return (envp);
}

t_env *ft_set(t_env *envp, char *key, char *value)
{
    if (is_duplicate_envp(envp, key))
        envp = update_env(envp, key, value);
    else
        envp = add_env(envp, make_env(key, value));
    ft_free(key);
    ft_free(value);
    return (envp);
}

t_env *update_envp(int argc, char **argv, t_env *envp, int *exit_status)
{
    int i;
    int sep;
    char *key;
    char *value;

    i = 0;
    sep = 1;
    while (argv[++i])
    {
        sep = find_sep(argv[i], '=');
        if (sep == -1)
            continue;
        key = ft_substr(argv[i], 0, sep);
        if (is_invalid_key(key, exit_status))
        {
            printf("bash: export: `%s': not a valid identifier\n", argv[i]);
            continue;
        }
        value = ft_substr(argv[i], sep + 1, ft_strlen(argv[i]));
        envp = ft_set(envp, key, value);
    }
    return (envp);
}

void ft_export(int argc, char **argv, t_env *envp, int *status)
{
    if (argc == 1) // export만 입력되었을 때
        print_sorted_envp(envp);
    else
        envp = update_envp(argc, argv, envp, status);
}

int main(int argc, char **argv, char **env)
{
    t_env *envp;
    int exit_status;

    exit_status = 0;
    envp = init_env(env);
    ft_export(argc, argv, envp, &exit_status);
    // /*test
    printf("=============================\n");
    while (envp)
    {
        printf("%s=%s\n", envp->key, envp->value);
        envp = envp->nxt;
    }
    // */
    exit(exit_status);
}