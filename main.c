/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:31:30 by hako              #+#    #+#             */
/*   Updated: 2022/07/28 18:10:01 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	g_stat;

int	proper_order(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if ((tmp->type == TRUNC || tmp->type == APPEND))
		{
			if (tmp->nxt && tmp->nxt->nxt && tmp->nxt->nxt->type == CMD)
				return (0);
		}
		tmp = tmp->nxt;
	}
	return (1);
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
	while (!proper_order(token))
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

void	print_lemon(void)
{
	char	buf[500001];
	int		length;
	int		fd;

	fd = open("lemon", O_RDONLY);
	length = read(fd, buf, 500000);
	write(1, buf, length);
	close(fd);
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
