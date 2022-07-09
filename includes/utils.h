#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>

size_t ft_strlen(const char *str);
char *ft_strdup(const char *src);
char *ft_strchr(char *s, int c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void *ft_malloc(size_t size);

#endif