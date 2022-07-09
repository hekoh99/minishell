/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:53 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/09 16:34:00 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env *init_env(char **env)
{
    int i = 1;
    t_env *lst;
    t_env *new;
    t_env *tmp;
    int j;

    lst = (t_env *)malloc(sizeof(t_env));
    lst->key = ft_strdup(env[0]);
    lst->value = ft_strdup(env[0]);
    lst->nxt = NULL;
    tmp = lst;
    while (env[i])
    {
        j = 0;
        while (env[i][j] != '=')
            j++;
        new = (t_env *)malloc(sizeof(t_env));
        new->key = ft_substr(env[i], 0, j);
        new->value = ft_substr(env[i], j + 1, ft_strlen(env[i]));
        new->nxt = NULL;
        tmp->nxt = new;
        tmp = tmp->nxt;
        i++;
    }
    return (lst);
}

void check_export_syntax(int argc, char *str)
{
    int sep;

    if (!ft_strchr(str, '=')) // 아무것도 하지 않음
        exit(0);
    else if (argc > 2 && ('0' > str[0] || str[0] > '9') && str[0] != '_')
    {
        printf("bash: export: `%s': not a valid identifier", str);
        free(str);
        exit(1);
    }
}

// t_env *sort_envp(t_env *envp)
// {
//     t_env *tmp;
//     t_env *left;
//     t_env *right;
// }

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

void print_all_envp(t_env *envp)
{
    t_env *sorted_envp;
    t_env *tmp;

    tmp = dup_envp(envp);
    // while (tmp)
    // {
    //     printf("%s=%s\n", tmp->key, tmp->value);
    //     tmp = tmp->nxt;
    // }
    // sorted_envp = sort_envp(envp);
}

void do_export(int argc, char **argv, t_env *envp)
{
    if (argc == 1)
        print_all_envp(envp);
    // check_export_syntax(argc, argv);
}

int main(int argc, char **argv, char **env)
{
    t_env *envp;

    envp = init_env(env);
    do_export(argc, argv, envp);
}