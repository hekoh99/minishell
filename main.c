/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:31:30 by hako              #+#    #+#             */
/*   Updated: 2022/07/06 16:18:39 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <termios.h>

// my mac : gcc main.c -lreadline -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include
// hako cluster : gcc main.c -lreadline -L/Users/hako/.brew/opt/readline/lib -I/Users/    hako/.brew/opt/readline/include

typedef struct s_env {
    char *value;
    struct s_env *nxt;
} t_env;

char	*ft_strdup(const char *src)
{
	int		index;
	char	*dest;

	index = 0;
	dest = (char *)malloc(strlen(src) * sizeof(char) + 1);
	if (!dest)
		return (0);
	while (src[index])
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (dest);
}

void sig_int(int signal)
{
    if (signal != SIGINT)
        return ;
    printf("minishell$ \n");
    rl_on_new_line(); // 개행이 나온 후 수행되어야함
    rl_replace_line("", 1); // 버퍼 비우고
    rl_redisplay(); // prompt
}

t_env *init_env(char **env)
{
    int i = 1;
    t_env *lst;
    t_env *new;
    t_env *tmp;

    lst = (t_env *)malloc(sizeof(t_env));
    lst->value = ft_strdup(env[0]);
    lst->nxt = NULL;
    tmp = lst;
    while (env[i])
    {
        new = (t_env *)malloc(sizeof(t_env));
        new->value = ft_strdup(env[i]);
        new->nxt = NULL;
        tmp->nxt = new;
        tmp = tmp->nxt;
        i++;
    }
    return (lst);
}

int main(int ac, char **av, char **env)
{
    char *str;
    t_env *envp;

    ac += 0;
    av += 0;

    signal(SIGINT, sig_int);
    signal(SIGQUIT, SIG_IGN);

    envp = init_env(env);
    /* // env init check
    while (envp){
        printf("out : %s\n", envp->value);
        envp = envp->nxt;
    }
    // */
    while (1)
    {
        str = readline("minishell$ ");
        if (!str || strcmp(str, "exit") == 0) // 나중에 libft 함수로 바꾸기
        {
            printf("\033[1A"); // 커서를 위로 하나 올리기
            printf("\033[11C"); // 10만큼 앞으로
            printf("exit\n");
            exit(0);
        }
        else if (*str == '\0')
        {
            free(str);
        }
        else
        {
            add_history(str);
            printf("%s\n", str);
            free(str);
        }
    }
    return (0);
}
