#ifndef MINISHELL_H
# define MINISHELL_H
 
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdio.h>
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

 #endif