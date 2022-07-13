/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 13:42:06 by yubin             #+#    #+#             */
/*   Updated: 2022/07/13 15:43:42 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

static int is_space(char c)
{
    if (c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == ' ')
        return (1);
    return (0);
}

int ft_atoi(const char *str)
{
    int i;
    long sign;
    long result;

    i = 0;
    while (is_space(str[i]))
        i++;
    sign = 1;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign *= -1;
        i++;
    }
    result = 0;
    while (ft_isdigit(str[i]))
        result = (result * 10) + (str[i++] - '0');
    if (result < 0)
        return ((sign + 1) / -2);
    return (result * sign);
}

long long ft_atoll(const char *str)
{
    int i;
    long long sign;
    long long result;

    i = 0;
    while (is_space(str[i]))
        i++;
    sign = 1;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign *= -1;
        i++;
    }
    result = 0;
    while (ft_isdigit(str[i]))
        result = (result * 10) + (str[i++] - '0');
    if (result < 0)
        return ((sign + 1) / -2);
    return (result * sign);
}
