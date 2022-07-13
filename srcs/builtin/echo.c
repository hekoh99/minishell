/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:59:25 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/07 17:00:07 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int main(int ac, char **av)
{
    int i;
    int option;

    i = 0;
    option = 0;
    while (++i < ac)
    {
        if (av[i][0] == '-' && is_option_n(av[i]))
            ++option;
        else
            break;
    }
    while (i < ac)
    {
        printf("%s", av[i++]);
        if (i != ac)
            printf(" ");
    }
    if (!option)
        printf("\n");
    return (0);
}