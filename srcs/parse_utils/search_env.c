/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:24:41 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 16:28:43 by hako             ###   ########.fr       */
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
