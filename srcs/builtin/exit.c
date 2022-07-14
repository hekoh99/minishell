/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 13:00:56 by yubin             #+#    #+#             */
/*   Updated: 2022/07/14 13:56:20 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

void validate_exit_code(char *code)
{
    int i;

    i = 0;
    if (code[i] == '-' || code[i] == '+')
        ++i;
    while (code && code[++i])
    {
        if (!ft_isdigit(code[i]))
        {
            printf("exit: %s: numeric argument required\n", code);
            exit(255);
        }
    }
}

void validate_argc(char **argv)
{
    if (argv[2] != NULL)
    {
        printf("exit: too many arguments\n");
        g_stat = 1;
    }
}

void ft_exit(t_node *node)
{
    long long status_ll;

    printf("exit\n"); // 명령어가 하나일 때 출력
    if (node->cmd[1] == NULL)
        exit(0);
    validate_exit_code(node->cmd[1]);
    status_ll = ft_atoll(node->cmd[1]);
    g_stat = status_ll % 256 + 256 * (status_ll < 0);
    if (node->cmd[2] != NULL)
    {
        printf("exit: too many arguments\n");
        g_stat = 1;
    }
    else
        exit(g_stat);
}