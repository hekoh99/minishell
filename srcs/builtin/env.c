/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 12:57:34 by yubin             #+#    #+#             */
/*   Updated: 2022/07/14 13:25:52 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_stat;

void ft_env(t_env *envp)
{
	while (envp)
	{
		printf("%s=%s\n", envp->key, envp->value);
		envp = envp->nxt;
	}
}