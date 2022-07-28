/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:53 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/28 00:53:57 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*ft_set(t_env *envp, char *key, char *value)
{
	envp = update_env(envp, key, value);
	ft_free(key);
	ft_free(value);
	return (envp);
}

char	*set_value(char *str, int sep)
{
	char	*value;

	if (!ft_strchr(str, '='))
		return (NULL);
	value = ft_substr(str, sep + 1, ft_strlen(str));
	return (value);
}

void	update_envp(t_node *node)
{
	int		i;
	int		sep;
	char	*key;
	char	*value;

	i = 0;
	sep = 1;
	while (node->cmd[++i])
	{
		sep = find_sep(node->cmd[i], '=');
		if (sep == 0)
		{
			print_error2(ft_strdup("export : `"),
				ft_strjoin(node->cmd[i], "': not a valid identifier\n"), 1);
			continue ;
		}
		key = ft_substr(node->cmd[i], 0, sep);
		if (is_invalid_key("export", key))
		{
			ft_free(key);
			continue ;
		}
		value = set_value(node->cmd[i], sep);
		node->envp = ft_set(node->envp, key, value);
	}
}

void	ft_export(t_node *node)
{
	if (node->cmd[1] == NULL)
		print_sorted_envp(node->envp);
	else
		update_envp(node);
}
