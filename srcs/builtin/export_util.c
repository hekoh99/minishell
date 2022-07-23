/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:29:18 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/23 16:51:44 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap_envp_var(t_env *left, t_env *right)
{
	t_env	tmp;

	tmp.key = left->key;
	tmp.value = left->value;
	left->key = right->key;
	left->value = right->value;
	right->key = tmp.key;
	right->value = tmp.value;
}

t_env	*sort_envp(t_env *envp)
{
	t_env	*tmp;
	t_env	*left;
	t_env	*right;

	tmp = envp;
	while (tmp)
	{
		left = tmp;
		right = tmp->nxt;
		while (right)
		{
			if (ft_strcmp(left->key, right->key) > 0)
				swap_envp_var(left, right);
			right = right->nxt;
		}
		tmp = tmp->nxt;
	}
	return (envp);
}

void	free_dup_envp(t_env *envp)
{
	while (envp)
	{
		ft_free(envp->key);
		ft_free(envp->value);
		ft_free(envp);
		envp = envp->nxt;
	}
	ft_free(envp);
}

void	print_sorted_envp(t_env *envp)
{
	t_env	*tmp;

	tmp = dup_envp(envp);
	tmp = sort_envp(tmp);
	print_all_envp(tmp, 1);
	free_dup_envp(tmp);
}
