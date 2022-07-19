/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:29:18 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 18:29:47 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*dup_env(t_env *env)
{
	t_env	*dup_env;

	dup_env = (t_env *)ft_malloc(sizeof(t_env));
	if (!env)
	{
		dup_env->key = NULL;
		dup_env->value = NULL;
		dup_env->nxt = NULL;
	}
	else
	{
		dup_env->key = ft_strdup(env->key);
		dup_env->value = ft_strdup(env->value);
		dup_env->nxt = NULL;
	}
	return (dup_env);
}

t_env	*dup_envp(t_env *envp)
{
	t_env	*dup_envp;
	t_env	*tmp;

	dup_envp = (t_env *)ft_malloc(sizeof(t_env));
	dup_envp = dup_env(envp);
	if (dup_envp->key)
	{
		tmp = dup_envp;
		while (envp->nxt)
		{
			envp = envp->nxt;
			tmp->nxt = (t_env *)ft_malloc(sizeof(t_env));
			tmp->nxt->key = ft_strdup(envp->key);
			tmp->nxt->value = ft_strdup(envp->value);
			tmp->nxt->nxt = NULL;
			tmp = tmp->nxt;
		}
	}
	return (dup_envp);
}

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
			if (ft_strncmp(left->key, right->key,
					select_longer(left->key, right->key)) > 0)
				swap_envp_var(left, right);
			right = right->nxt;
		}
		tmp = tmp->nxt;
	}
	return (envp);
}

void	print_sorted_envp(t_env *envp)
{
	t_env	*sorted_envp;
	t_env	*tmp;

	tmp = dup_envp(envp);
	tmp = sort_envp(tmp);
	print_all_envp(tmp, 1);
}
