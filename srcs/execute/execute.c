/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/16 10:37:50 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

void ft_buitlin(t_node *node)
{
	if (ft_strcmp(node->cmd[0], "cd") == 0)
		ft_cd(node);
	else if (ft_strcmp(node->cmd[0], "echo") == 0)
		ft_echo(node);
	else if (ft_strcmp(node->cmd[0], "env") == 0)
		ft_env(node->envp);
	else if (ft_strcmp(node->cmd[0], "exit") == 0)
		ft_exit(node);
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

void ft_execve(t_node *node)
{
	return;
}

void ft_command(t_node *node)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork error", 1);
	else if (pid == 0)
	{
		// dup2
		if (is_builtin(node))
			ft_buitlin(node);
		else
			ft_execve(node);
		exit(g_stat);
	}
	else
	{
		if (node->fd[IN] != 0)
			close(node->fd[IN]);
		if (node->fd[OUT] != 1)
			close(node->fd[OUT]);
	}
}

void ft_pipe(t_node *node)
{
}

void ft_execute(t_node *node)
{
	pid_t pid;

	if (!has_pipe(node) && is_builtin(node))
		ft_buitlin(node);
	else
	{
		while (node)
		{
			if (node->type == CMD)
				ft_command(node);
			else if (node->type == PIPE)
				ft_pipe(node);
			node = node->nxt;
		}
		while (wait((int *)0) != -1)
			;
	}
}