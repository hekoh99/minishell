/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:43:04 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/23 18:43:23 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../includes/minishell.h"

void	free_token_value(t_token *token)
{
	free(token->value);
	free(token);
}

t_token	*do_ft_dellist(t_token **head, t_token *tmp)
{
	t_token	*del;

	if (tmp->prev == NULL)
	{
		*head = tmp->nxt;
		free_token_value(tmp);
		tmp = *head;
		tmp->prev = NULL;
	}
	else if (tmp->nxt == NULL)
	{
		tmp = tmp->prev;
		free_token_value(tmp->nxt);
		tmp->nxt = NULL;
	}
	else
	{
		del = tmp;
		tmp->prev->nxt = tmp->nxt;
		tmp->nxt->prev = tmp->prev;
		tmp = tmp->prev;
		free_token_value(del);
	}
	return (tmp);
}

t_token	*ft_dellist(t_token *head, char *target)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		if (ft_strcmp(tmp->value, target) == 0)
			tmp = do_ft_dellist(&head, tmp);
		tmp = tmp->nxt;
	}
	return (head);
}

void	free_token_all(t_token *head)
{
	t_token	*tmp;
	t_token	*target;

	if (!head)
		return ;
	tmp = head;
	while (tmp)
	{
		target = tmp;
		if (target->value)
			free(target->value);
		tmp = tmp->nxt;
		free(target);
	}
}

void	free_node_all(t_node *head)
{
	t_node	*tmp;
	t_node	*target;
	int		i;

	if (!head)
		return ;
	tmp = head;
	while (tmp)
	{
		target = tmp;
		if (target->fd[IN] != -1 && target->fd[IN] != 0)
			close(target->fd[IN]);
		if (target->fd[OUT] != -1 && target->fd[OUT] != 1)
			close(target->fd[OUT]);
		if (target->cmd)
		{
			i = -1;
			while (target->cmd[++i])
				free(target->cmd[i]);
			free(target->cmd);
		}
		tmp = tmp->nxt;
		free(target);
	}
}
