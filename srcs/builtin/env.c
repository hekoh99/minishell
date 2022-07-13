/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 12:57:34 by yubin             #+#    #+#             */
/*   Updated: 2022/07/12 13:31:48 by yubin            ###   ########.fr       */
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

void do_env(t_env *envp)
{
    t_env *tmp;

    tmp = envp;
    while (tmp)
    {
        printf("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->nxt;
    }
}

int main(int ac, char **av, char **env)
{
    t_env *envp;

    envp = init_env(env);
    if (ac > 1)
        exit(1);
    do_env(envp);
    exit(0);
}