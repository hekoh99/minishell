/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/14 12:16:39 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_buitlin(t_node *node)
{
	if (ft_strcmp(node->cmd[0], "cd") == 0)
		ft_cd(node);
	else if (ft_strcmp(node->cmd[0], "echo") == 0)
		ft_echo(node);
	else if (ft_strcmp(node->cmd[0], "env") == 0)
		ft_env(node);
	else if (ft_strcmp(node->cmd[0], "exit") == 0)
		ft_exit(node);
	else if (ft_strcmp(node->cmd[0], "pwd") == 0)
		ft_pwd(node);
	else if (ft_strcmp(node->cmd[0], "unset") == 0)
		ft_unset(node);
}

int is_builtin(t_node *node)
{
	if (!ft_strcmp(node->cmd[0], "cd") || !ft_strcmp(node->cmd[0], "echo") ||
		!ft_strcmp(node->cmd[0], "env") || !ft_strcmp(node->cmd[0], "exit") ||
		!ft_strcmp(node->cmd[0], "pwd") || !ft_strcmp(node->cmd[0], "unset"))
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
	if (is_builtin(node))
		ft_buitlin(node);
	else
		ft_exec(node);
}

int ft_execute(t_node *node)
{
	pid_t pid;

	if (!has_pipe(node) && is_builtin(node))
		ft_buitlin(node);
	while (node)
	{
		if (node->type == CMD)
		{
			pid = fork();
			if (pid == -1)
				error_exit("fork error", 1);
			else if (pid == 0)
				ft_command(node);
			else
				do_parent();
		}
		node = node->nxt;
	}
}