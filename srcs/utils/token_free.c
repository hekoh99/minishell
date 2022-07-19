/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:43:04 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 18:43:50 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../includes/minishell.h"

t_token	*ft_dellist(t_token *head, char *target)
{
	t_token	*tmp;
	t_token	*del;

	tmp = head;
	while (tmp)
	{
		if (ft_strcmp(tmp->value, target) == 0)
		{
			if (tmp->prev == NULL)
			{
				head = tmp->nxt;
				free(tmp->value);
				free(tmp);
				tmp = head;
				tmp->prev = NULL;
			}
			else if (tmp->nxt == NULL)
			{
				tmp = tmp->prev;
				free(tmp->nxt->value);
				free(tmp->nxt);
				tmp->nxt = NULL;
			}
			else
			{
				del = tmp;
				tmp->prev->nxt = tmp->nxt;
				tmp->nxt->prev = tmp->prev;
				tmp = tmp->prev;
				free(del->value);
				free(del);
			}
		}
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
		{
			free(target->value);
		}
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
			i = 0;
			while (target->cmd[i])
			{
				free(target->cmd[i]);
				i++;
			}
			free(target->cmd);
		}
		tmp = tmp->nxt;
		free(target);
	}
}

void	free_env_all(t_env *head)
{
	t_env	*tmp;
	t_env	*target;

	if (!head)
		return ;
	tmp = head;
	while (tmp)
	{
		target = tmp;
		if (target->key)
			free(target->key);
		if (target->value)
			free(target->value);
		tmp = tmp->nxt;
		free(target);
	}
}
