/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:58:13 by yubin             #+#    #+#             */
/*   Updated: 2022/07/09 15:58:13 by yubin            ###   ########.fr       */
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
