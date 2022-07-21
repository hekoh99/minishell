/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:58:13 by yubin             #+#    #+#             */
/*   Updated: 2022/07/21 13:32:46 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

int	ft_strncmp(char *s1, char *s2, int n)
{
	if (!s1 || !s2)
		return (-1);
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

int	ft_strcmp(char *s1, char *s2)
{
	int	n;

	n = select_longer(s1, s2);
	return (ft_strncmp(s1, s2, n));
}
