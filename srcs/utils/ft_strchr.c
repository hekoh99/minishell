/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:58:04 by yubin             #+#    #+#             */
/*   Updated: 2022/07/21 13:33:16 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

void	*ft_memchr(const void *src, int c, size_t n)
{
	size_t	index;

	index = 0;
	if (!src)
		return (NULL);
	while (index < n)
	{
		if (((unsigned char *)src)[index] == (unsigned char)c)
			return (((unsigned char *)src) + index);
		index++;
	}
	return (0);
}

char	*ft_strchr(const char *s, int c)
{
	return (ft_memchr(s, c, ft_strlen(s) + 1));
}
