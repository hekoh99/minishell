/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 12:57:34 by yubin             #+#    #+#             */
/*   Updated: 2022/07/09 13:24:28 by yubin            ###   ########.fr       */
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

void do_env(t_env *envp)
{
    t_env *tmp;

    tmp = envp;
    while (tmp)
    {
        printf("%s\n", tmp->value);
        tmp = tmp->nxt;
    }
}

int main(int ac, char **av, char **env)
{
    t_env *envp;

    envp = init_env(env);
    if (ac > 1)
    {
        printf("env: %s: No such file or directory\n", av[1]);
        exit(1);
    }
    do_env(envp);
    exit(0);
}