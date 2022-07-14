/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:44:10 by yubin             #+#    #+#             */
/*   Updated: 2022/07/14 14:12:33 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

extern int g_stat;

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

void printf_invalid_identifier(char *key)
{
    g_stat = 1;
    printf("export: `%s': not a valid identifier\n", key);
    ft_free(key);
}

int is_invalid_key(char *key)
{
    int i;

    if (ft_strlen(key) == 1 && key[0] == '_')
        return (0);
    i = -1;
    while (key[++i])
    {
        if (!ft_isalpha(key[i]) && key[i] != '_')
        {
            if (i > 0 && ft_isdigit(key[i]))
                continue;
            printf_invalid_identifier(key);
            return (1);
        }
    }
    return (0);
}