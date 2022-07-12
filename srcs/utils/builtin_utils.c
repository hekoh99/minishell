/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:44:10 by yubin             #+#    #+#             */
/*   Updated: 2022/07/12 15:55:52 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

int select_longer(char *str1, char *str2)
{
    int str1_len;
    int str2_len;

    str1_len = ft_strlen(str1);
    str2_len = ft_strlen(str2);
    if (str1_len > str2_len)
        return (str1_len);
    return (str2_len);
}

int is_invalid_key(char *key, int *exit_status)
{
    int i;

    *exit_status = 0;
    if (ft_strlen(key) == 1 && key[0] == '_')
        return (0);
    i = -1;
    while (key[++i])
    {
        if (!ft_isalpha(key[i]) && key[i] != '_')
        {
            if (i > 0 && ft_isdigit(key[i]))
                continue;
            *exit_status = 1;
            return (1);
        }
    }
    return (0);
}