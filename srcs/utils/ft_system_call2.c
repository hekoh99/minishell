/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system_call2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 10:45:29 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/16 14:26:50 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

extern int g_stat;

void	ft_execve(char *file, char **argv, char **envp)
{
	char *msg;

	if (!file)
	{
		msg = ft_strjoin(argv[0], ": command not found\n");
		write(2, msg, ft_strlen(msg));
		g_stat = 127;
	}
	else if (execve(file, argv, envp) == -1)
		error_exit(argv[0], 1);
}