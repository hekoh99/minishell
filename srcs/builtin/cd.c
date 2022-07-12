/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 14:18:06 by yubin             #+#    #+#             */
/*   Updated: 2022/07/12 16:06:54 by yubin            ###   ########.fr       */
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

char *search_env_value(t_env *env, char *target)
{
    char *real_val;
    int i;
    int find;

    real_val = NULL;
    while (env)
    {
        if (ft_strncmp(env->key, target, select_longer(env->key, target)) == 0) // 환경변수 찾음
        {
            real_val = ft_substr(env->value, 0, ft_strlen(env->value));
            break;
        }
        env = env->nxt;
    }
    return (real_val);
}

void ft_cd_home(t_env *envp, int *status)
{
    char *home_path;

    home_path = search_env_value(envp, "HOME");
    if (!home_path)
    {
        printf("cd: HOME not set\n");
        *status = 1;
        return;
    }
    if (chdir(home_path) == -1)
    {
        printf("cd: %s: no such file or directory\n", home_path);
        *status = 1;
        return;
    }
}

void ft_cd(int argc, char **argv, t_env *envp, int *status)
{
    if (argc == 1)
        ft_cd_home(envp, status);
    else
    {
        if (chdir(argv[1]) == -1)
        {
            printf("cd: %s: no such file or directory\n", argv[1]);
            *status = 1;
        }
    }
}

int main(int argc, char **argv, char **env)
{
    t_env *envp;
    int exit_status;

    exit_status = 0;
    envp = init_env(env);
    ft_cd(argc, argv, envp, &exit_status);

    // test
    char path[PATH_MAX];

    getcwd(path, PATH_MAX);
    printf("%s\n", path);
    // */
    exit(exit_status);
}