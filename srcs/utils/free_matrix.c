/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 12:38:37 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 18:37:23 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = -1;
	if (matrix)
	{
		while (matrix[++i])
			ft_free(matrix[i]);
		ft_free(matrix);
	}
}
