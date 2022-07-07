/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:05:29 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/07 17:15:51 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

char *ft_strchr(char *s, int c)
{
    int i;

    i = 0;
    if (!s)
        return (NULL);
    while (s[i])
    {
        if (s[i] == (char)c)
            return (&(s[i]));
        ++i;
    }
    if (s[i] == (char)c)
        return (&(s[i]));
    return (NULL);
}
