/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/28 15:00:43 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

pid_t	ft_command(t_node *node)
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
	return (pid);
}

void	make_status(int child)
{
	g_stat = WEXITSTATUS(child);
	if (WTERMSIG(child) == SIGINT)
		g_stat = WTERMSIG(child) + 128;
	if (WTERMSIG(child) == SIGQUIT)
		g_stat = WTERMSIG(child) + 128;
}

void	ft_execute(t_node *node)	// todo: norm 맞춰서 정리 필요~
{
	int		child;
	int		nchild;
	pid_t	pid;
	int		result;
	int		i;

	nchild = 0;
	child = 0;
	if (is_single_cmd(node) && is_builtin(node))
	{
		if (ft_strcmp(node->cmd[0], "exit"))
			g_stat = 0;
		ft_buitlin(SINGLE_CMD, node);
	}
	else
	{
		while (node)
		{
			signal(SIGINT, child_sig_int);
			signal(SIGQUIT, sigquit);
			if (node->type == CMD && ++nchild)
				pid = ft_command(node);
			node = node->nxt;
		}
		i = 0;
		while (i < nchild)
		{
			if (wait(&child) == pid)
				result = child;
			i++;
		}
		if (nchild > 0)
			make_status(result);
	}
}
