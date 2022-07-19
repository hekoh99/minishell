/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 12:12:30 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 18:37:06 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

extern int	g_stat;

void	print_error(char *msg, int status)
{
	write(2, "minishell: ", 11);
	write(2, msg, ft_strlen(msg));
	g_stat = status;
	ft_free(msg);
}

void	print_error2(char *function, char *msg, int status)
{
	write(2, "minishell: ", 11);
	write(2, function, ft_strlen(function));
	write(2, msg, ft_strlen(msg));
	g_stat = status;
	ft_free(function);
	ft_free(msg);
}

void	error_exit(char *msg, int status)
{
	perror(msg);
	g_stat = status;
	exit(g_stat);
}
