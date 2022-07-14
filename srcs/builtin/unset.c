/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:15:38 by yubin             #+#    #+#             */
/*   Updated: 2022/07/14 12:46:35 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

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
        if (ft_strncmp(tmp->key, key, select_longer(tmp->key, key)) == 0)
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

t_env *ft_unset(t_node *node, t_env *envp)
{
    int i;

    i = -1;
    ;
    while (node && node->cmd && node->cmd[++i])
    {
        if (is_invalid_key(ft_strdup(node->cmd[i]), g_stat))
            continue;
        envp = ft_unset_env(envp, node->cmd[i]);
        node = node->nxt;
    }
    return (envp);
}