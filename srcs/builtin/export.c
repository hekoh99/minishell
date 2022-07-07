/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:53 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/07 17:22:43 by yubchoi          ###   ########.fr       */
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

void do_export(int argc, char *str, t_env *envp)
{
    check_export_syntax(argc, str);
}

void fork_and_execve(int argc, char **argv, t_env *envp)
{
    if (ft_strncmp(argv[1], "export", 6) && ft_strlen(argv[1]) == 6)
        do_export(argc, argv, envp);
    else
        printf("command error\n");
}

int main(int argc, char **argv, char **env)
{
    t_env *envp;

    envp = init_env(env);
    fork_and_execve(argc, argv, envp);
}