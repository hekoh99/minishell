/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 13:00:56 by yubin             #+#    #+#             */
/*   Updated: 2022/07/13 15:43:14 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void validate_argc(int argc, long long *status)
{
    if (argc > 2)
    {
        printf("exit: too many arguments\n");
        *status = 1;
    }
}

void ft_exit(int argc, char **argv, long long *status)
{
    /* 명령어가 하나라면 */
    if (1) // TODO: cmd->nxt == NULL로 변경
        printf("exit\n");
    if (argc == 1)
        exit(0);
    validate_exit_code(argv[1]);
    *status = ft_atoll(argv[1]);
    *status %= 256 + 256 * (*status < 0);
    validate_argc(argc, status);
}

int main(int argc, char **argv)
{
    long long exit_status;

    exit_status = 0;
    ft_exit(argc, argv, &exit_status);
    /* 부모 프로세스 */
    if (exit_status == 1)
        return (exit_status);
    else
        exit(exit_status);
}