/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:30:05 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/28 00:54:30 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	find_sep(char *str, int sep)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == sep)
			return (i);
		++i;
	}
	return (i);
}

t_env	*make_env(char *key, char *value)
{
	t_env	*env;

	env = (t_env *)ft_malloc(sizeof(t_env));
	env->key = ft_strdup(key);
	if (!value)
		env->value = NULL;
	else
		env->value = ft_strdup(value);
	env->nxt = NULL;
	return (env);
}

t_env	*add_env(t_env *envp, t_env *env)
{
	t_env	*tmp;

	if (!envp)
		return (env);
	tmp = envp;
	while (tmp->nxt)
		tmp = tmp->nxt;
	tmp->nxt = env;
	return (envp);
}

int	is_duplicate_envp(t_env *envp, char *key)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (1);
		tmp = tmp->nxt;
	}
	return (0);
}

t_env	*update_env(t_env *envp, char *key, char *value)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (!value)
				break;
			if (tmp->value)
				ft_free(tmp->value);
			else
				tmp->value = ft_strdup(value);
			break ;
		}
		tmp = tmp->nxt;
	}
	if (!tmp)
		envp = add_env(envp, make_env(key, value));
	return (envp);
}
