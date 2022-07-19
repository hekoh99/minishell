/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system_call2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 10:45:29 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 18:42:40 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

extern int	g_stat;

void	ft_execve(char *file, char **argv, char **envp)
{
	if (!file)
		print_error2(ft_strdup(argv[0]),
			ft_strdup(": No such file or directory\n"), 127);
	else if (!argv || !*argv || !*argv[0])
		print_error(ft_strjoin(argv[0], ": command not found\n"), 127);
	else if (execve(file, argv, envp) == -1)
		print_error(ft_strjoin(argv[0], ": is a directory\n"), 126);
}
