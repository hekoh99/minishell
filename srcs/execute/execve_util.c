/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:33:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/19 18:35:18 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**find_path(t_node *node)
{
	int		i;
	char	**path;
	t_env	*envp;

	i = -1;
	path = NULL;
	envp = node->envp;
	while (envp)
	{
		if (ft_strncmp(envp->key, "PATH", 4) == 0)
		{
			path = ft_split((envp->value), ':');
			return (path);
		}
		envp = envp->nxt;
	}
	free_matrix(path);
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;
	char	*tmp_cmd;

	i = -1;
	if (ft_strchr(cmd, '/'))
		return (cmd);
	while (paths && paths[++i])
	{
		tmp_cmd = ft_strjoin("/", cmd);
		cmd_path = ft_strjoin(paths[i], tmp_cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			free(tmp_cmd);
			return (cmd_path);
		}
		free(tmp_cmd);
		free(cmd_path);
	}
	return (NULL);
}
