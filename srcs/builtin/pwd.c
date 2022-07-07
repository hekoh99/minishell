#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int main(void)
{
    char path[PATH_MAX];

    getcwd(path, PATH_MAX);
    printf("%s\n", path);
    return (0);
}