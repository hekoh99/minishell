/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 12:14:26 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/21 13:48:43 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_envp(t_env *envp)
{
	int	i;

	i = 0;
	while (envp)
	{
		++i;
		envp = envp->nxt;
	}
	return (i);
}

char	**init_envp_arr(t_env *envp)
{
	int		i;
	char	**envp_arr;

	i = -1;
	envp_arr = (char **)malloc(sizeof(char *) * (count_envp(envp) + 1));
	while (envp)
	{
		envp_arr[++i] = ft_strjoin(envp->key, "=");
		envp_arr[i] = ft_strjoin(envp_arr[i], envp->value);
		envp = envp->nxt;
	}
	envp_arr[++i] = NULL;
	return (envp_arr);
}

void	init_arg(t_node *node, t_exec_arg *arg)
{
	char	**paths;

	paths = find_path(node);
	arg->argv = node->cmd;
	arg->file = find_cmd_path(arg->argv[0], paths);
	arg->envp = init_envp_arr(node->envp);
	free_matrix(paths);
}

void	do_execve(t_node *node)
{
	t_exec_arg	arg;

	if (!node->cmd || !node->cmd[0])
		return ;
	init_arg(node, &arg);
	ft_execve(arg.file, arg.argv, arg.envp);
	free_matrix(arg.envp);
}
