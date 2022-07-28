/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:29:32 by hako              #+#    #+#             */
/*   Updated: 2022/07/28 01:03:59 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

void	sigquit(int signal)
{
	if (signal != SIGQUIT)
		return ;
	g_stat = 2;
	write(2, "^\\Quit: 3\n", 10);
}

void	child_sig_int(int signal)
{
	if (signal != SIGINT)
		return ;
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
	write(1, "\033[u", 3);
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	write(1, "\033[s", 3);
}

void	heredoc_sig_int(int signal)
{
	struct termios	attributes;
	struct termios	saved;

	tcgetattr(STDIN_FILENO, &saved);
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= (~ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	if (signal != SIGINT)
		return ;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	g_stat = ETC;
}
