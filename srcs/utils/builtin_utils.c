/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:44:10 by yubin             #+#    #+#             */
/*   Updated: 2022/07/28 00:59:07 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

int	select_longer(char *str1, char *str2)
{
	int	str1_len;
	int	str2_len;

	str1_len = ft_strlen(str1);
	str2_len = ft_strlen(str2);
	if (str1_len > str2_len)
		return (str1_len);
	return (str2_len);
}

int	is_invalid_key(char *function, char *key)
{
	int	i;

	if (ft_strlen(key) == 1 && key[0] == '_')
		return (0);
	i = -1;
	while (key[++i])
	{
		if (!ft_isalpha(key[i]) && key[i] != '_')
		{
			if (i > 0 && ft_isdigit(key[i]))
				continue ;
			print_error2(ft_strjoin(function, ": `"),
				ft_strjoin(key, "': not a valid identifier\n"), 1);
			return (1);
		}
	}
	return (0);
}

void	print_all_envp(t_env *envp, int has_prefix)
{
	while (envp)
	{
		if (has_prefix && !ft_strcmp("_", envp->key))
			envp = envp->nxt;
		else if (has_prefix && envp->value && *envp->value)
			printf("declare -x %s=\"%s\"\n", envp->key, envp->value);
		else if (has_prefix && envp->value)
			printf("declare -x %s=\"\"\n", envp->key);
		else if (has_prefix)
			printf("declare -x %s\n", envp->key);
		else if (envp->value && envp->value)
			printf("%s=%s\n", envp->key, envp->value);
		envp = envp->nxt;
	}
}
