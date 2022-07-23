/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:24:41 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 14:48:52 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*search_env(t_env *env, char *target)
{
	char	*real_val;

	real_val = NULL;
	while (env)
	{
		if (ft_strcmp(env->key, target) == 0)
		{
			real_val = ft_substr(env->value, 0, ft_strlen(env->value));
			break ;
		}
		env = env->nxt;
	}
	return (real_val);
}
