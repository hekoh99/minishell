/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:10:11 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/21 13:35:13 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_buitlin(int single_cmd, t_node *node)
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

int	is_builtin(t_node *node)
{
	if (!ft_strcmp(node->cmd[0], "cd") || !ft_strcmp(node->cmd[0], "echo")
		|| !ft_strcmp(node->cmd[0], "env") || !ft_strcmp(node->cmd[0], "exit")
		|| !ft_strcmp(node->cmd[0], "export") || !ft_strcmp(node->cmd[0], "pwd")
		|| !ft_strcmp(node->cmd[0], "unset"))
		return (1);
	return (0);
}

int	is_single_cmd(t_node *node)
{
	if (!node->nxt)
		return (1);
	return (0);
}

void	close_pipe(t_node *node)
{
	t_node	*tmp;

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

void	clean_fd(void)
{
	int	i;

	i = 3;
	while (i < OPEN_MAX)
		close(i++);
}
