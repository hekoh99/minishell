/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:33:19 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/23 19:34:46 by yubchoi          ###   ########.fr       */
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
	print_error2(ft_strdup(node->cmd[0]),
		ft_strdup(": No such file or directory\n"), 127);
	free_matrix(path);
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;
	char	*tmp_cmd;
	int		fd;

	i = -1;
	if (ft_strchr(cmd, '/'))
		return (cmd);
	while (paths && paths[++i])
	{
		tmp_cmd = ft_strjoin("/", cmd);
		cmd_path = ft_strjoin(paths[i], tmp_cmd);
		fd = open(cmd_path, O_EXCL);
		if (fd > 0)
		{
			close(fd);
			free(tmp_cmd);
			return (cmd_path);
		}
		close(fd);
		free(tmp_cmd);
		free(cmd_path);
	}
	return (NULL);
}
