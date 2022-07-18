/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 13:00:56 by yubin             #+#    #+#             */
/*   Updated: 2022/07/18 14:08:55 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

void validate_exit_code(char *code)
{
    int i;

    i = -1;
    if (code[0] == '-' || code[0] == '+')
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

void ft_exit(int single_cmd, t_node *node)
{
    long long status_ll;

    if (single_cmd)
        printf("exit\n");
    if (node->cmd[1] == NULL)
    {
        printf("g_stat: %d\n", g_stat); // test
        exit(g_stat);
    }
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