/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:40:07 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/27 13:04:25 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

static size_t	num_len(long number)
{
	size_t	size;

	if (number <= 0)
	{
		number *= -1;
		size = 1;
	}
	else
		size = 0;
	while (number > 0)
	{
		number /= 10;
		size++;
	}
	return (size);
}

static	void	set_zero_nega(char *a, long *number)
{
	if (*number == 0)
		*a = '0';
	if (*number < 0)
	{
		*a = '-';
		*number *= -1;
	}
}

char	*ft_itoa(int n)
{
	long	number;
	char	*array;
	size_t	size;

	number = n;
	size = num_len(n);
	array = (char *)malloc((size + 1) * sizeof(char));
	if (!array)
		return (0);
	array[size] = '\0';
	set_zero_nega(&array[0], &number);
	if (n == 0)
		return (array);
	while (number > 0)
	{
		array[size - 1] = (number % 10) + '0';
		number /= 10;
		size--;
	}
	return (array);
}
