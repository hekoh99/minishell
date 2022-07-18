/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system_call2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 10:45:29 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/18 14:01:29 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

extern int g_stat;

void ft_execve(char *file, char **argv, char **envp)
{
	char *msg;

	if (!file)
		print_error2(ft_strdup(argv[0]), ft_strdup(": No such file or directory\n"), 127);
	else if (execve(file, argv, envp) == -1)
		print_error(ft_strjoin(argv[0], ": is a directory\n"), 126);
}