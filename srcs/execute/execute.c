/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/27 13:04:17 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

void	ft_command(t_node *node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork error", 1);
	else if (pid == 0)
	{
		g_stat = 0;
		dup2(node->fd[IN], 0);
		dup2(node->fd[OUT], 1);
		close_pipe(node);
		if (is_builtin(node))
			ft_buitlin(MULTI_CMD, node);
		else
			do_execve(node);
		exit(g_stat);
	}
	else
	{
		if (node->fd[IN] != 0)
			ft_close(node->fd[IN]);
		if (node->fd[OUT] != 1)
			ft_close(node->fd[OUT]);
	}
}

void	make_status(int child)
{
	g_stat = WEXITSTATUS(child);
	if (WTERMSIG(child) == SIGINT)
		g_stat = WTERMSIG(child) + 128;
}

void	ft_execute(t_node *node)
{
	int		child;
	int		nchild;

	nchild = 0;
	child = 0;
	if (is_single_cmd(node) && is_builtin(node))
	{
		g_stat = 0;
		ft_buitlin(SINGLE_CMD, node);
	}
	else
	{
		while (node)
		{
			signal(SIGINT, child_sig_int);
			if (node->type == CMD && ++nchild)
				ft_command(node);
			node = node->nxt;
		}
		while (wait(&child) != -1)
			;
		if (nchild > 0)
			make_status(child);
	}
}
