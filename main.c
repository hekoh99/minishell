/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:31:30 by hako              #+#    #+#             */
/*   Updated: 2022/07/27 14:57:17 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	g_stat;

void	handle_eof_exit(t_env *envp)
{
	write(1, "\033[1A", 4);
	write(1, "\033[11C", 5);
	write(1, "exit\n", 5);
	free_env_all(envp);
	exit(g_stat);
}

t_token	*do_parse(char *str, t_env *envp)
{
	t_token	*token;

	token = NULL;
	token = trim_space(str);
	free(str);
	token = split_by_sep(token);
	token = add_type(token);
	token = expand(token, envp);
	token = trim_quote(token);
	token = reorder_token(token);
	return (token);
}

void	minishell(t_env *envp)
{
	char	*str;
	t_token	*token;
	t_node	*node;

	token = NULL;
	node = NULL;
	write(1, "\033[s", 3);
	write(1, "\033[u", 3);
	str = readline("minishell$ ");
	if (!str)
		handle_eof_exit(envp);
	else if (*str == '\0')
		free(str);
	else
	{
		add_history(str);
		token = do_parse(str, envp);
		node = exec_unit(&token, envp);
		if (node)
			ft_execute(node);
		free_token_all(token);
		free_node_all(node);
		tmp_files(NULL, DEL);
	}
}

void	write_with_color(int fd, char *str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '+')
		{
			write(1, "\033[31m빨간 휴지 줄까??? \033[34m파랑 휴지 줄까???\033[0m", ft_strlen("\033[31m빨간 휴지 줄까??? \033[34m파랑 휴지 줄까???\033[0m"));
			i++;
			write(fd, &str[i], 1);
			i++;
			write(fd, &str[i], 1);
			i++;
		}
		else
		{
			write(fd, &str[i], 1);
			i++;
		}
	}
}

void	print_with_color(int fd, char *str, int len)
{
	int	i;

	i = -1;
	while (++i < len)
	{
		if (str[i] == '+')
			printf("\033[31m+\033[0m");
		else if (str[i] == '*')
			printf("%c", str[i]);
	}
}

void	print_lemon(void)
{
	char	buf[500001];
	int		length;
	int		fd;
	
	fd = open("lemon", O_RDONLY);
	length = read(fd, buf, 500000);
	write(1, buf, length);
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_env	*envp;

	ac += 0;
	av += 0;
	envp = init_env(env);
	g_stat = 0;
	print_lemon();
	while (1)
	{
		signal(SIGINT, sig_int);
		signal(SIGQUIT, SIG_IGN);
		minishell(envp);
	}
	return (0);
}