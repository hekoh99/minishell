#include "../includes/minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

extern int g_stat;

void child_sig_int(int signal)
{
    write(2, "^C\n", 3);
}

void heredoc_sig_int(int signal)
{
    if (signal != SIGINT)
        return;
    ioctl(STDIN_FILENO, TIOCSTI, "\n");
    rl_on_new_line();       // 개행이 나온 후 수행되어야함
    rl_replace_line("", 1); // 버퍼 비우고
    g_stat = ETC;
}