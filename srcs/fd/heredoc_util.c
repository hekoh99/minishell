/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 11:25:48 by hako              #+#    #+#             */
/*   Updated: 2022/07/27 11:26:00 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cursor_up(void)
{
	write(1, "\033[1A", 4);
	write(1, "\033[2C", 4);
	write(1, "\033[s", 3);
	return (1);
}

int	at_heredoc_sigint(char *str, char *here_str)
{	
	free(str);
	free(here_str);
	tmp_files(NULL, DEL);
	return (0);
}
