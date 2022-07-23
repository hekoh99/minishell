/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 13:00:56 by yubin             #+#    #+#             */
/*   Updated: 2022/07/23 13:23:47 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

void	validate_exit_code(char *code)
{
	int	i;

	i = -1;
	if (code[0] == '-' || code[0] == '+')
		++i;
	while (code && code[++i])
	{
		if (!ft_isdigit(code[i]))
		{
			print_error2(ft_strjoin("exit: ", code), ft_strdup(": numeric argument required\n"), 255);
			exit(255);
		}
	}
}

void	ft_exit(int single_cmd, t_node *node)
{
	long long	status_ll;

	if (single_cmd)
		printf("exit\n");
	if (node->cmd[1] == NULL)
		exit(g_stat);
	validate_exit_code(node->cmd[1]);
	status_ll = ft_atoll(node->cmd[1]);
	g_stat = status_ll % 256 + 256 * (status_ll < 0);
	if (node->cmd[2] != NULL)
	{
		print_error(ft_strdup("exit: too many arguments\n"), 1);
		g_stat = 1;
	}
	else
		exit(g_stat);
}
