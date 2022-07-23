/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:25:37 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 14:55:51 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

t_node	*new_node(int type, int size, t_env *envp)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	new->type = type;
	new->cmd = (char **)malloc(sizeof(char *) * (size + 1));
	new->cmd[size] = NULL;
	new->nxt = NULL;
	new->prev = NULL;
	new->fd[IN] = 0;
	new->fd[OUT] = 1;
	new->envp = envp;
	return (new);
}

t_node	*add_node(t_node *head, t_token *target, int iter, t_env *envp)
{
	t_node	*new;
	t_node	*tmp;

	new = new_node(target->type, iter, envp);
	new = add_cmd_arr(new, target, iter);
	if (head == NULL)
		head = new;
	else
	{
		tmp = head;
		while (tmp->nxt)
			tmp = tmp->nxt;
		tmp->nxt = new;
		new->prev = tmp;
	}
	return (head);
}

t_node	*add_node_by_type(t_node *head,
		t_token **token, t_token **tmp, t_env *envp)
{
	t_token		*start;
	int			is_error;

	start = *tmp;
	if (*token)
	{
		is_error = error_handler(head, token, tmp);
		if (is_error == -1)
		{
			free_node_all(head);
			return (NULL);
		}
	}
	if ((*tmp)->type == PIPE)
		head = add_node(head, start, 1, envp);
	else if ((*tmp)->type == END)
		head = add_node(head, start, 1, envp);
	else if ((*tmp)->type == TRUNC || (*tmp)->type == APPEND
		|| (*tmp)->type == INPUT || (*tmp)->type == HEREDOC)
	{
		head = add_node(head, start, 2, envp);
		(*tmp) = (*tmp)->nxt;
	}
	(*tmp) = (*tmp)->nxt;
	return (head);
}

t_node	*get_exec_unit(t_node *head, t_token **token,
		t_token **tmp, t_env *envp)
{
	int		i;
	t_token	*start;

	i = 0;
	start = *tmp;
	if ((*tmp)->type == CMD)
	{
		while ((*tmp) && (*tmp)->type == CMD)
		{
			(*tmp) = (*tmp)->nxt;
			i++;
		}
		head = add_node(head, start, i, envp);
	}
	else
	{
		head = add_node_by_type(head, token, tmp, envp);
		if (!head)
			return (NULL);
	}
	return (head);
}

t_node	*exec_unit(t_token **token, t_env *envp)
{
	t_node	*head;
	t_token	*tmp;

	head = NULL;
	tmp = *token;
	while (tmp)
	{
		head = get_exec_unit(head, token, &tmp, envp);
		if (!head)
			return (NULL);
	}
	head = get_fd(head);
	return (head);
}
