/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:47:02 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 16:53:55 by hako             ###   ########.fr       */
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
