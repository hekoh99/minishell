/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:58:04 by yubin             #+#    #+#             */
/*   Updated: 2022/07/09 15:58:05 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

void *ft_memchr(const void *src, int c, size_t n)
{
    size_t index;

    index = 0;
    while (index < n)
    {
        if (((unsigned char *)src)[index] == (unsigned char)c)
            return (((unsigned char *)src) + index);
        index++;
    }
    return (0);
}

char *ft_strchr(const char *s, int c)
{
    return (ft_memchr(s, c, ft_strlen(s) + 1));
}
