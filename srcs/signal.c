#include "../includes/minishell.h"

extern int g_stat;

void child_sig_int(int signal)
{
    if (signal != SIGINT)
        return;
    g_stat = ETC;
    // rl_redisplay();         // prompt
}