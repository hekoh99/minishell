/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:20:42 by hako              #+#    #+#             */
/*   Updated: 2022/07/27 11:38:31 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_node	*set_cmd_pipe(t_node *prev, t_node *tmp, t_node *cmd)
{
	prev->fd[OUT] = tmp->fd[OUT];
	if (cmd && cmd->fd[OUT] == 1)
		cmd->fd[OUT] = tmp->fd[OUT];
	if (!cmd || cmd->type == FAIL)
		close(tmp->fd[OUT]);
	return (NULL);
}

void	set_cmd_output(t_node *tmp, t_node *cmd)
{
	if (tmp->nxt && tmp->nxt->type == PIPE)
		ft_close(tmp->nxt->fd[OUT]);
	if (cmd)
		cmd->fd[OUT] = tmp->fd[OUT];
}

t_node	*init_cmd(t_node *prev, t_node *cmd)
{
	if (prev && prev->type > CMD)
		cmd->fd[IN] = prev->fd[IN];
	if (cmd->nxt == NULL)
		cmd->fd[OUT] = 1;
	return (cmd);
}

void	do_set_cmd_fd(t_node **cmd, t_node *prev, t_node *tmp)
{
	if (prev && prev->type == END)
		prev = NULL;
	if (tmp->type == CMD)
		*cmd = init_cmd(prev, tmp);
	if (*cmd && (tmp->type == INPUT || tmp->type == HEREDOC))
	{
		if (tmp->fd[IN] == -1)
			(*cmd)->type = FAIL;
		(*cmd)->fd[IN] = tmp->fd[IN];
	}
	if (tmp->type == TRUNC || tmp->type == APPEND)
		set_cmd_output(tmp, *cmd);
	if (prev && tmp->type == PIPE)
		(*cmd) = set_cmd_pipe(prev, tmp, *cmd);
}

t_node	*get_fd(t_node *node)
{
	if (!set_separator_fd(node))
		return (NULL);
	set_cmd_fd(node);
	return (node);
}
