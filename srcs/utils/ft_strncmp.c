/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:58:13 by yubin             #+#    #+#             */
/*   Updated: 2022/07/14 11:32:17 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    while (n > 0 && *s1 == *s2 && *s1 != '\0')
    {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
        return (0);
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t n;

    n = select_longer(s1, s2);
    ft_strncmp(s1, s2, n);
}