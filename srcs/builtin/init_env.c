/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:20:19 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 12:31:08 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*env_block(char **env, int i)
{
	int		j;
	t_env	*new;

	j = 0;
	while (env[i][j] != '=')
		j++;
	new = (t_env *)malloc(sizeof(t_env));
	new->key = ft_substr(env[i], 0, j);
	new->value = ft_substr(env[i], j + 1, ft_strlen(env[i]));
	new->nxt = NULL;
	return (new);
}

t_env	*init_env(char **env)
{
	int		i;
	t_env	*lst;
	t_env	*new;
	t_env	*tmp;

	lst = NULL;
	tmp = lst;
	i = 0;
	while (env[i])
	{
		new = env_block(env, i);
		if (!lst)
		{
			lst = new;
			tmp = lst;
			i++;
			continue ;
		}
		else
			tmp->nxt = new;
		tmp = tmp->nxt;
		i++;
	}
	return (lst);
}
