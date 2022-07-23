/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:29:32 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 14:50:03 by yubchoi          ###   ########.fr       */
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

void	sig_int(int signal)
{
	struct termios	attributes;
	struct termios	saved;

	tcgetattr(STDIN_FILENO, &saved);
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= (~ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	if (signal != SIGINT)
		return ;
	g_stat = ETC;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
}

void	heredoc_sig_int(int signal)
{
	if (signal != SIGINT)
		return ;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	g_stat = ETC;
}
