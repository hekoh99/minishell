#include "../includes/minishell.h"
#include <sys/ioctl.h>
#include <termios.h>

extern int g_stat;

void child_sig_int(int signal)
{
    write(2, "^C\n", 3);
}