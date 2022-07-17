/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system_call.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:07:41 by yubin             #+#    #+#             */
/*   Updated: 2022/07/16 10:47:56 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_free(void *str)
{
    free(str);
    str = 0;
}

void *ft_malloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (ptr == NULL)
    {
        printf("Error: malloc failed\n");
        exit(1);
    }
    return (ptr);
}

int	ft_open(char *file, int o_flag, int mode)
{
	int	file_fd;

	if (mode == -1)
		file_fd = open(file, o_flag);
	else
		file_fd = open(file, o_flag, mode);
	if (file_fd == -1)
		error_exit("No such file or directory", 1);
	return (file_fd);
}

void	ft_close(int fd)
{
	// printf("close fd : %d\n", fd);
	if (close(fd) == -1)
		error_exit("close failed", 1);
}

void	ft_dup2(int fd, int fd2)
{
	if (dup2(fd, fd2) == -1)
		error_exit("dup2 failed", 1);
}

