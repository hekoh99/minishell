/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 11:25:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/14 13:10:41 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_buitlin(t_mini *mini)
{
	if (ft_strcmp(mini->node->cmd[0], "cd") == 0)
		ft_cd(mini);
	// else if (ft_strcmp(mini->node->cmd[0], "echo") == 0)
	// 	ft_echo(mini->node);
	// else if (ft_strcmp(mini->node->cmd[0], "env") == 0)
	// 	ft_env(mini->node);
	// else if (ft_strcmp(mini->node->cmd[0], "exit") == 0)
	// 	ft_exit(mini->node);
	// else if (ft_strcmp(mini->node->cmd[0], "pwd") == 0)
	// 	ft_pwd(mini->node);
	// else if (ft_strcmp(mini->node->cmd[0], "unset") == 0)
	// 	ft_unset(mini->node, mini->envp);
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

void ft_execve(t_mini *mini)
{
}

void ft_command(t_mini *mini)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork error", 1);
	else if (pid == 0)
	{
		if (is_builtin(mini->node))
			ft_buitlin(mini);
		else
			ft_execve(mini);
	}
	else
		;
}

void ft_pipe(t_node *node)
{
}

void ft_execute(t_mini *mini)
{
	pid_t pid;
	t_node *node;

	node = mini->node;
	if (!has_pipe(node) && is_builtin(node))
		ft_buitlin(mini);
	else
	{
		while (node)
		{
			if (node->type == CMD)
				ft_command(mini);
			else if (node->type == PIPE)
				ft_pipe(node);
			node = node->nxt;
		}
		while (wait((int *)0) != -1)
			;
	}
}