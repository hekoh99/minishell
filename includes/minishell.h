#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

#define CMD 1
#define PIPE 2
#define TRUNC 3
#define APPEND 4
#define INPUT 5
#define HEREDOC 6
#define END 7

#define GET 0
#define ADD 1
#define DEL 2

// exit status
#define SYNTAX 258
#define CMD_NOT_FOUND 127
#define INTERRUPT 130
#define PERM 126
#define ETC 1

#define IN 0
#define OUT 1

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *nxt;
} t_env;

typedef struct s_token
{
    char *value;
    int type;
    struct s_token *nxt;
    struct s_token *prev;
} t_token;

typedef struct s_node
{
    char **cmd;
    int type;
    int fd[2];
    struct s_node *nxt;
} t_node;

typedef struct s_list
{
    char *value;
    struct s_list *nxt;
} t_list;

typedef struct s_mini
{
    t_env *envp;
    t_node *node;
} t_mini;

t_token *trim_space(char *line);
t_token *split_by_sep(t_token *token);
t_token *expand(t_token *token, t_env *env);
t_token *trim_quote(t_token *token);
t_token *add_type(t_token *token);
t_node *exec_unit(t_token **token);
t_list *tmp_files();
void delete_files();

// token utils
t_token *ft_dellist(t_token *head, char *target);
void free_token_all(t_token *head);
void free_node_all(t_node *head);

// execute
void ft_execute(t_mini *mini);

// builtin
void ft_cd(t_mini *mini);
void ft_echo(t_mini *mini);
void ft_env(t_env *envp);
void ft_exit(t_node *node);

#endif
