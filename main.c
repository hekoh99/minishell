/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubin <yubchoi@student.42>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:31:30 by hako              #+#    #+#             */
/*   Updated: 2022/07/18 14:09:38 by yubin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

int g_stat;

void sig_int(int signal)
{
    if (signal != SIGINT)
        return;
    g_stat = ETC;
    ioctl(STDIN_FILENO, TIOCSTI, "\n");
    rl_on_new_line();       // 개행이 나온 후 수행되어야함
    rl_replace_line("", 1); // 버퍼 비우고
    // rl_redisplay();         // prompt
}

t_env *init_env(char **env)
{
    int i = 0;
    t_env *lst;
    t_env *new;
    t_env *tmp;
    int j;

    lst = NULL;
    tmp = lst;
    while (env[i])
    {
        j = 0;
        while (env[i][j] != '=')
            j++;
        new = (t_env *)malloc(sizeof(t_env));
        new->key = ft_substr(env[i], 0, j);
        new->value = ft_substr(env[i], j + 1, ft_strlen(env[i]));
        new->nxt = NULL;
        if (!lst)
        {
            lst = new;
            tmp = lst;
            i++;
            continue;
        }
        else
            tmp->nxt = new;
        tmp = tmp->nxt;
        i++;
    }
    return (lst);
}

// test code
void print_token(t_token *token, int flag)
{
    char type[7][10] = {"CMD", "PIPE", "TRUNC", "APPEND", "INPUT", "HEREDOC", "END"};
    while (token && flag == 1)
    {
        printf("[%s : %s]", token->value, type[token->type - 1]);
        token = token->nxt;
    }
    while (token && flag == 0)
    {
        printf("[%s]", token->value);
        token = token->nxt;
    }
    printf("\n");
}

// test code
void print_node(t_node *node)
{
    int i;
    char type[7][10] = {"CMD", "PIPE", "TRUNC", "APPEND", "INPUT", "HEREDOC", "END"};

    while (node)
    {
        i = 0;
        printf("-------- %s ----------\n", type[node->type - 1]);
        while (node->cmd && node->cmd[i])
        {
            printf("%s\n", node->cmd[i]);
            i++;
        }
        printf("infile : %d\n", node->fd[IN]);
        printf("outfile : %d\n", node->fd[OUT]);
        node = node->nxt;
    }
}

// test code
void print_heredoc(t_node *node)
{
    char *line;

    if (!node)
        return;

    while (node)
    {
        if (node->type == HEREDOC)
        {
            line = get_next_line(node->fd[IN]);
            printf("%d\n", node->fd[IN]);
            printf("------- heredoc -------\n");
            while (line)
            {
                printf("line : %s\n", line);
                free(line);
                line = get_next_line(node->fd[IN]);
            }
        }
        if (node)
            node = node->nxt;
    }
}

// test code
void print_tmpfiles()
{
    t_list *tmp = tmp_files(NULL, GET);
    printf("------ tmps files ------\n");
    while (tmp)
    {
        printf("%s\n", tmp->value);
        tmp = tmp->nxt;
    }
}

int main(int ac, char **av, char **env)
{
    // t_mini mini;
    char *str;
    t_env *envp;
    t_token *token;
    t_node *node;
    struct termios attributes;
    struct termios saved;

    ac += 0;
    av += 0;

    envp = init_env(env);
    g_stat = 0;
    /* // env init check
    while (envp){
        printf("%s = %s\n", envp->key, envp->value);
        envp = envp->nxt;
    }
    // */
    while (1)
    {
        token = NULL;
        node = NULL;
        signal(SIGINT, sig_int);
        signal(SIGQUIT, SIG_IGN);
        tcgetattr(STDIN_FILENO, &saved);
        tcgetattr(STDIN_FILENO, &attributes);
        attributes.c_lflag &= (~ECHOCTL);
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        str = readline("minishell$ ");
        if (!str)
        {
            printf("\033[1A");  // 커서를 위로 하나 올리기
            printf("\033[11C"); // 10만큼 앞으로
            printf("exit\n");
            free_env_all(envp);
            exit(g_stat);
        }
        else if (*str == '\0')
        {
            free(str);
        }
        else
        {
            add_history(str);
            // printf("parrot : %s\n", str);
            token = trim_space(str); // 따옴표 검사까지 token 값 0 이면 error
            token = split_by_sep(token);
            token = add_type(token);
            token = expand(token, envp);
            token = trim_quote(token);
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
            free_token_all(token);
            free_node_all(node);
            tmp_files(NULL, DEL);
        }
        // if (tmp_files(NULL, GET) == NULL)
        //     printf("yes\n");
        // printf("stat : %d\n", g_stat);
    }
    return (0);
}