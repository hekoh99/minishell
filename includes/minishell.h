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
# include "../libft/libft.h"

# define CMD 1
# define PIPE 2
# define REDIR 3
# define VAR 4

typedef struct s_env {
    char *value;
    struct s_env *nxt;
} t_env;
 
typedef struct s_token {
    char *value;
    int type;
    struct s_token *nxt;
    struct s_token *prev;
} t_token;

// typedef stuct s_mini{
//     t_env *env;
//     t_env *token;
// } t_mini;

t_token *trim_space(char *line);

 #endif