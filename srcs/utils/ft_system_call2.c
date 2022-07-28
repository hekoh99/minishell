/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system_call2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 10:45:29 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/28 17:54:40 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

void	ft_execve(char *file, char **argv, char **envp)
{
	if (!argv || !*argv || !*argv[0] || !file)
		print_error(ft_strjoin(argv[0], ": command not found\n"), 127);
	else if (execve(file, argv, envp) == -1)
		print_error(ft_strjoin(argv[0], ": No such file or directory\n"), 127);
}

void	free_env_all(t_env *head)
{
	t_env	*tmp;
	t_env	*target;

	if (!head)
		return ;
	tmp = head;
	while (tmp)
	{
		target = tmp;
		if (target->key)
			free(target->key);
		if (target->value)
			free(target->value);
		tmp = tmp->nxt;
		free(target);
	}
}
