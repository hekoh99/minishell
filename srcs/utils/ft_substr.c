/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:36:38 by hako              #+#    #+#             */
/*   Updated: 2022/07/19 18:42:13 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	s_len;
	size_t	index;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	str = (char *)ft_malloc(len * sizeof(char) + 1);
	index = 0;
	while (index < len && index + start < s_len)
	{
		str[index] = s[start + index];
		index++;
	}
	str[index] = '\0';
	return (str);
}
