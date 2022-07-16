/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/16 15:23:13 by yubchoi          ###   ########.fr       */
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

int has_pipe(t_node *node)
{
	while (node)
	{
		if (ft_strcmp(node->cmd[0], "|") == 0)
			return (1);
		node = node->nxt;
	}
	return (0);
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
		// if (node->prev && node->prev->type == PIPE)
		// {
		// 	printf("%d %d\n", node->prev->fd[OUT], node->prev->fd[IN]);
		// 	close(node->prev->fd[IN]);
		// 	close(node->prev->fd[OUT]);
		// }
	}
}

void ft_execute(t_node *node)
{
	pid_t pid;
	int tmp;

	g_stat = 0;
	if (!has_pipe(node) && is_builtin(node))
		ft_buitlin(SINGLE_CMD, node);
	else
	{
		while (node)
		{
			if (node->type == CMD)
				ft_command(node);
			node = node->nxt;
		}
		while (wait(&tmp) != -1)
		{
			if (WIFEXITED(tmp))
				g_stat = WEXITSTATUS(tmp);
			else
				g_stat = WTERMSIG(tmp);
		}
	}
}