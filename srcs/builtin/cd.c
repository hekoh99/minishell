/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 14:18:06 by yubin             #+#    #+#             */
/*   Updated: 2022/07/15 14:12:05 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

char *search_env_value(t_env *env, char *target)
{
    char *value;
    int i;
    int find;

    value = NULL;
    while (env)
    {
        if (ft_strcmp(env->key, target) == 0)
        {
            value = ft_substr(env->value, 0, ft_strlen(env->value));
            break;
        }
        env = env->nxt;
    }
    return (value);
}

void ft_cd_home(t_env *envp)
{
    char *home_path;

    home_path = search_env_value(envp, "HOME");
    if (!home_path)
    {
        printf("cd: HOME not set\n");
        g_stat = 1;
    }
    else if (chdir(home_path) == -1)
    {
        printf("cd: %s: no such file or directory\n", home_path);
        g_stat = 1;
    }
}

void ft_cd(t_node *node)
{
    char *old_pwd;
    char *pwd;

    old_pwd = getcwd(0, PATH_MAX);
    if (node->cmd[1] == NULL)
        ft_cd_home(node->envp);
    else
    {
        if (chdir(node->cmd[1]) == -1)
        {
            printf("cd: %s: no such file or directory\n", node->cmd[1]);
            g_stat = 1;
        }
    }
    pwd = getcwd(0, PATH_MAX);
    node->envp = update_env(node->envp, "OLDPWD", old_pwd);
    node->envp = update_env(node->envp, "PWD", pwd);
}