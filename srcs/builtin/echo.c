/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:59:25 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/14 13:23:11 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

int is_option_n(char *opt)
{
    int i;

    if (ft_strlen(opt) < 2)
        return (0);
    i = 1;
    while (opt && opt[i])
    {
        if (opt[i] != 'n')
            return (0);
        else
            ++i;
    }
    return (1);
}

void ft_echo(t_mini *mini)
{
    int i;
    int option;
    t_node *node;

    i = 0;
    option = 0;
    node = mini->node;
    while (node->cmd[++i])
    {
        if (node->cmd[i][0] == '-' && is_option_n(node->cmd[i]))
            ++option;
        else
            break;
    }
    while (node->cmd[i])
    {
        printf("%s", node->cmd[i++]);
        if (node->cmd[i] != NULL)
            printf(" ");
    }
    if (!option)
        printf("\n");
    g_stat = 0;
}