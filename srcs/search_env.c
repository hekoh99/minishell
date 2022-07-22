/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 19:42:09 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/21 19:42:09 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int g_stat;

char *search_env(t_env *env, char *target)
{
    char *real_val;
    int i;
    int find;

    real_val = NULL;
    while (env)
    {
        if (ft_strcmp(env->key, target) == 0)
        {
            real_val = ft_substr(env->value, 0, ft_strlen(env->value));
            break;
        }
        env = env->nxt;
    }
    return (real_val);
}