/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:15:38 by yubin             #+#    #+#             */
/*   Updated: 2022/07/21 16:26:46 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

t_env	*ft_unset_env(t_env *envp, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = envp;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->nxt = tmp->nxt;
			else
				envp = tmp->nxt;
			ft_free(tmp->key);
			ft_free(tmp->value);
			ft_free(tmp);
			return (envp);
		}
		prev = tmp;
		tmp = tmp->nxt;
	}
	return (envp);
}

void	ft_unset(t_node *node)
{
	int	i;

	i = 0;
	while (node && node->cmd && node->cmd[++i])
	{
		if (is_invalid_key("unset", node->cmd[i]))
			continue ;
		node->envp = ft_unset_env(node->envp, node->cmd[i]);
	}
}
