/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:15:38 by yubin             #+#    #+#             */
/*   Updated: 2022/07/12 14:14:25 by yubin            ###   ########.fr       */
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

t_env *ft_unset_env(t_env *envp, char *key)
{
    t_env *tmp;
    t_env *prev;

    tmp = envp;
    prev = NULL;
    while (tmp)
    {
        if (ft_strncmp(tmp->key, key, select_bigger(ft_strlen(tmp->key), ft_strlen(key))) == 0)
        {
            if (prev)
                prev->nxt = tmp->nxt;
            else
                envp = tmp->nxt;
            ft_free(tmp->key);
            ft_free(tmp->value);
            ft_free(tmp);
            return (envp);
        }
        prev = tmp;
        tmp = tmp->nxt;
    }
    return (envp);
}

t_env *ft_unset(t_env *envp, char **argv, int *status)
{
    int i;

    i = 0;
    while (argv && argv[++i])
    {
        if (is_invalid_key(argv[i], status))
            continue;
        envp = ft_unset_env(envp, argv[i]);
    }
    return (envp);
}

int main(int argc, char **argv, char **env)
{
    t_env *envp;
    int exit_status;

    exit_status = 0;
    envp = init_env(env);
    envp = ft_unset(envp, argv, &exit_status);
    // // test
    // while (envp)
    // {
    //     printf("%s=%s\n", envp->key, envp->value);
    //     envp = envp->nxt;
    // }
    // // */
    return (exit_status);
}