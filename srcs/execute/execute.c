/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 17:13:00 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

void ft_buitlin(int single_cmd, t_node *node)
{
	if (ft_strcmp(node->cmd[0], "cd") == 0)
		ft_cd(node);
	else if (ft_strcmp(node->cmd[0], "echo") == 0)
		ft_echo(node);
	else if (ft_strcmp(node->cmd[0], "env") == 0)
		ft_env(node->envp);
	else if (ft_strcmp(node->cmd[0], "exit") == 0)
		ft_exit(single_cmd, node);
	else if (ft_strcmp(node->cmd[0], "export") == 0)
		ft_export(node);
	else if (ft_strcmp(node->cmd[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(node->cmd[0], "unset") == 0)
		ft_unset(node);
}

int is_builtin(t_node *node)
{
	if (!ft_strcmp(node->cmd[0], "cd") || !ft_strcmp(node->cmd[0], "echo") ||
		!ft_strcmp(node->cmd[0], "env") || !ft_strcmp(node->cmd[0], "exit") ||
		!ft_strcmp(node->cmd[0], "export") || !ft_strcmp(node->cmd[0], "pwd") ||
		!ft_strcmp(node->cmd[0], "unset"))
		return (1);
	return (0);
}

int is_single_cmd(t_node *node)
{
	if (!node->nxt)
		return (1);
	return (0);
}

void close_pipe(t_node *node)
{
	t_node *tmp;

	tmp = node->nxt;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (tmp->prev && tmp->prev->type == CMD)
			{
				if (tmp->fd[OUT] > 2)
					ft_close(tmp->fd[OUT]);
				if (tmp->fd[IN] > 2)
					ft_close(tmp->fd[IN]);
			}
		}
		tmp = tmp->nxt;
	}
}

void ft_command(t_node *node)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork error", 1);
	else if (pid == 0)
	{
		g_stat = 0;
		ft_dup2(node->fd[IN], 0);
		ft_dup2(node->fd[OUT], 1);
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

void ft_redirection(t_node *node)
{
	t_node *tmp;

	tmp = node->nxt;
	if (tmp && tmp->type == CMD)
	{
		tmp->fd[OUT] = node->fd[OUT];
		tmp->fd[IN] = node->fd[IN];
	}
}

void ft_execute(t_node *node)
{
	pid_t pid;
	int tmp;

	if (is_single_cmd(node) && is_builtin(node))
		ft_buitlin(SINGLE_CMD, node);
	else
	{
		while (node)
		{
			signal(SIGINT, child_sig_int);
			if (node->type == CMD)
				ft_command(node);
			// else if (node->type == TRUNC || node->type == APPEND)
			// 	ft_redirection(node);
			node = node->nxt;
		}
		while (wait(&tmp) != -1)
			;
		g_stat = WEXITSTATUS(tmp);
		if (WTERMSIG(tmp) == SIGINT)
			g_stat = WTERMSIG(tmp) + 128;
	}
}