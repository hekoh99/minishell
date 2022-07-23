/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 22:21:04 by hako              #+#    #+#             */
/*   Updated: 2022/07/23 16:45:21 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_stat;

static int	open_tmpfile(void)
{
	int		fd;
	int		num;
	char	*file;
	char	*num_str;

	num = 1;
	file = ft_strdup(".tmp");
	fd = open(file, O_WRONLY | O_CREAT | O_EXCL, 0666);
	while (fd == -1)
	{
		free(file);
		num_str = ft_itoa(num);
		file = ft_strjoin(".tmp", num_str);
		free(num_str);
		fd = open(file, O_WRONLY | O_CREAT | O_EXCL, 0666);
		num++;
	}
	tmp_files(file, ADD);
	return (fd);
}

static char	*get_heredoc_str(char *here_str, char *readline)
{
	char	*tmp;

	tmp = readline;
	readline = ft_strjoin(tmp, "\n");
	free(tmp);
	tmp = here_str;
	here_str = ft_strjoin(tmp, readline);
	free(tmp);
	free(readline);
	return (here_str);
}

static int	get_heredoc_readend(int wrfd, char *here_str)
{
	int		fd;
	char	*file;

	file = tmp_files(NULL, GET)->value;
	write(wrfd, here_str, ft_strlen(here_str));
	close(wrfd);
	fd = open(file, O_RDONLY, 0666);
	free(here_str);
	if (g_stat == ETC)
	{
		tmp_files(NULL, DEL);
		close(fd);
		return (-1);
	}
	return (fd);
}

int	get_heredoc_fd(t_node *node)
{
	int		fd;
	char	*here_str;
	char	*str;

	here_str = ft_strdup("");
	signal(SIGINT, heredoc_sig_int);
	g_stat = 0;
	fd = open_tmpfile();
	while (g_stat != ETC)
	{
		str = readline("> ");
		if (!str)
		{
			printf("\033[1A");	// 커서를 위로 하나 올리기
			printf("\033[2C"); // 10만큼 앞으로
			break ;
		}
		if (ft_strcmp(str, node->cmd[1]) == 0)
		{
			free(str);
			break ;
		}
		here_str = get_heredoc_str(here_str, str);
	}
	return (get_heredoc_readend(fd, here_str));
}
