/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:29:32 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 13:26:50 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

extern int	g_stat;

void	child_sig_int(int signal)
{
    if (signal == SIGINT)
        write(2, "^C\n", 3);
}
