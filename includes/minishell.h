#ifndef MINISHELL_H
# define MINISHELL_H
 
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

#include "utils.h"

#define CMD 1
#define PIPE 2
#define REDIR 3
#define VAR 4

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

// enum e_builtin
// {
//     ECHO = 1,
//     CD = 2,
//     PWD = 3,
//     EXPORT = 4,
//     UNSET = 5,
//     ENV = 6,
//     EXIT = 7
// };

// typedef stuct s_mini{
//     t_env *env;
//     t_env *token;
// } t_mini;

t_token *trim_space(char *line);
t_token *split_by_sep(t_token *token);
t_token *expand(t_token *token, t_env *env);

// token utils
t_token *ft_dellist(t_token *head, char *target);
void free_token_all(t_token *head);

 #endif
