/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:20:54 by hako              #+#    #+#             */
/*   Updated: 2022/07/27 12:11:13 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

int	set_input_fd(t_node *head, t_node *file_node)
{
	if (file_node->type == INPUT)
	{
		file_node->fd[IN] = open(file_node->cmd[1], O_RDONLY);
		if (file_node->fd[IN] == -1)
		{
			printf("minishell: %s: No such file or directory\n",
				file_node->cmd[1]);
			g_stat = ETC;
			return (1);
		}
	}
	else
		file_node->fd[IN] = get_heredoc_fd(file_node);
	if (file_node->type == HEREDOC && g_stat == ETC)
		free_node_all(head);
	return (1);
}

int	set_output_fd(t_node *head, t_node *file_node)
{
	if (file_node->type == APPEND)
	{
		file_node->fd[OUT] = ft_open(file_node->cmd[1],
				O_CREAT | O_WRONLY | O_APPEND, 0666);
		if (file_node->fd[OUT] == -1)
		{
			free_node_all(head);
			return (0);
		}
	}
	else if (file_node->type == TRUNC)
	{
		file_node->fd[OUT] = ft_open(file_node->cmd[1],
				O_CREAT | O_WRONLY | O_TRUNC, 0666);
		if (file_node->fd[OUT] == -1)
		{
			free_node_all(head);
			return (0);
		}
	}
	return (1);
}

int	set_separator_fd(t_node *node)
{
	t_node	*tmp;
	int		type;

	tmp = node;
	while (tmp)
	{
		if (tmp->type == INPUT || tmp->type == HEREDOC)
		{
			type = tmp->type;
			if (set_input_fd(node, tmp) == 0
				|| (type == HEREDOC && g_stat == ETC))
				return (0);
		}
		else if ((tmp->type == TRUNC || tmp->type == APPEND)
			&& set_output_fd(node, tmp) == 0)
			return (0);
		else if (tmp->type == PIPE)
		{
			pipe(tmp->fd);
			tmp->nxt->fd[IN] = tmp->fd[IN];
		}
		tmp = tmp->nxt;
	}
	return (1);
}

int	set_cmd_fd(t_node *node)
{
	t_node	*tmp;
	t_node	*prev;
	t_node	*cmd;

	tmp = node;
	prev = NULL;
	cmd = NULL;
	while (tmp)
	{
		do_set_cmd_fd(&cmd, prev, tmp);
		prev = tmp;
		tmp = tmp->nxt;
	}
	return (1);
}
