/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:31:30 by hako              #+#    #+#             */
/*   Updated: 2022/07/19 18:44:31 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int g_stat;

void sig_int(int signal)
{
	struct termios attributes;
	struct termios saved;

	tcgetattr(STDIN_FILENO, &saved);
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= (~ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	if (signal != SIGINT)
		return ;
	g_stat = ETC;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
}

void heredoc_sig_int(int signal)
{
	if (signal != SIGINT)
		return ;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	g_stat = ETC;
}

int main(int ac, char **av, char **env)
{
	char *str;
	t_env *envp;
	t_token *token;
	t_node *node;

	ac += 0;
	av += 0;

	envp = init_env(env);
	g_stat = 0;
	while (1)
	{
		token = NULL;
		node = NULL;
		signal(SIGINT, sig_int);
		signal(SIGQUIT, SIG_IGN);
		str = readline("minishell$ ");
		if (!str)
		{
			printf("\033[1A");	// 커서를 위로 하나 올리기
			printf("\033[11C"); // 10만큼 앞으로
			printf("exit\n");
			free_env_all(envp);
			exit(g_stat);
		}
		else if (*str == '\0')
			free(str);
		else
		{
			add_history(str);
			token = trim_space(str);
			free(str);
			token = split_by_sep(token);
			token = add_type(token);
			token = expand(token, envp);
			token = trim_quote(token);
			token = reorder_token(token);
			node = exec_unit(&token, envp);
			// print_token(token, 1);
			// print_node(node);
			// print_heredoc(node);
			// print_tmpfiles();
			if (node)
				ft_execute(node);
			// print_token(token, 1);
			// print_heredoc(node);
			// print_tmpfiles();
			free_token_all(token); // 나중에 exec_unit 안에 넣기
			free_node_all(node);
			tmp_files(NULL, DEL);
		}
	}
	return (0);
}