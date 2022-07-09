/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:53 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/09 15:55:04 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env *init_env(char **env)
{
    int i = 1;
    t_env *lst;
    t_env *new;
    t_env *tmp;

    lst = (t_env *)malloc(sizeof(t_env));
    lst->value = ft_strdup(env[0]);
    lst->nxt = NULL;
    tmp = lst;
    while (env[i])
    {
        new = (t_env *)malloc(sizeof(t_env));
        new->value = ft_strdup(env[i]);
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

t_env *sort_envp(t_env *envp)
{
    t_env *tmp;
    t_env *left;
    t_env *right;
}

t_env *dup_env(t_env *envp)
{
    t_env *dup_env;

    dup_env = (t_env *)ft_malloc(sizeof(t_env));
    if (!envp)
    {
        dup_env->key = NULL;
        dup_env->value = NULL;
        dup_env->nxt = NULL;
    }
    else
    {
        dup_env->key = ft_strdup(envp->key);
        dup_env->value = ft_strdup(envp->value);
        dup_env->nxt = NULL;
    }
    return (dup_env);
}

t_env *dup_envp(t_env *envp)
{
    t_env *dup_envp;
    t_env new;
    t_env tmp;
}

void print_all_envp(t_env *envp)
{
    t_env *sorted_envp;
    t_env *tmp;

    tmp = dup_envp(envp);
    sorted_envp = sort_envp(envp);
}

void do_export(int argc, char **argv, t_env *envp)
{
    if (argc == 1)
        print_all_envp(envp);
    check_export_syntax(argc, argv);
}

int main(int argc, char **argv, char **env)
{
    t_env *envp;

    envp = init_env(env);
    do_export(argc, argv, envp);
}